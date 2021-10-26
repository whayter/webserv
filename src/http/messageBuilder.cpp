#include "ServerConfig.hpp"
#include "Context.hpp"
#include "Response.hpp"
#include "Status.hpp"
#include "messageParser.hpp"
#include "messageBuilder.hpp"
#include "cgi.hpp"
#include "HtmlBuilder.hpp"

#include <fstream>

namespace fs = ft::filesystem;

namespace http {

Response buildResponse(Request& request)
{
	Response response;
	Context ctxt = getContext(request.getUri());

	if (request.getHeader("Connection") == "close")
		response.setHeader("Connection", "close");
	if (!ctxt.location.hasLimitExceptMethod(request.getMethod()))
		return errorResponse(ctxt, response, Status::MethodNotAllowed);
	if (request.getMethod() == "DELETE")
		return deleteResponse(ctxt, response);
	if (request.getMethod() == "POST")
		return postResponse(ctxt, request, response);
	if (ctxt.location.getAction() == action::cgi)
		return dynamicResponse(ctxt, request, response);
	if (ctxt.location.getAction() == action::returnDirective)
		return redirectResponse(ctxt, response);
	if (ctxt.location.getAction() == action::none)
	{
		ft::error_code ec;
		fs::file_status stat = fs::status(ctxt.path, ec);
		if (ec.value() == ft::errc::no_such_file_or_directory)
			return errorResponse(ctxt, response, Status::NotFound);
		else if (ec)
			throw std::logic_error("Houston, we have a problem (" + ec.message() + ')');			
		else if (stat.type() == fs::file_type::regular)
			return staticResponse(ctxt, response);
		else if (stat.type() == fs::file_type::directory && ctxt.location.hasAutoindex())
			return autoIndexResponse(ctxt, request, response);
	}
	return errorResponse(ctxt, response, Status::NotFound);
}

Response staticResponse(const Context& ctxt, Response& response)
{
	std::string extension = ServerConfig::getInstance().getMime(ctxt.path.extension().c_str());
	response.setStatus(Status::OK);
	response.setContent(getFileContent(ctxt.path), extension);
	return response;
}

Response dynamicResponse(const Context& ctxt, Request& request, Response& response)
{
	setEnvironment(request, ctxt.server, ctxt.path);
	std::string cgiExecPath = ctxt.server.findLocation(ctxt.path.c_str()).getCgiExec();
	std::pair<content_type, ft::error_code> cgiPair = getCgiResponse(cgiExecPath);
	unsetEnvironment();
	if (cgiPair.second.value() != 0)
		return errorResponse(ctxt, response, Status::InternalServerError);
	Message cgiResponse = parseCgiResponse(cgiPair.first);
	std::string cgiStatus = cgiResponse.getHeader("Status");
	int status = strtol(cgiStatus.c_str(),  NULL, 10);
	if (isError(Status(status)))
		return (errorResponse(ctxt, response, Status(status)));
	if (status == 0)
		response.setStatus(Status::OK);
	else
		response.setStatus(Status(status));
	response.setContent(cgiResponse.getContent(), cgiResponse.getHeader("Content-type"));
	return response;
}

Response postResponse(const Context& ctxt, Request& request, Response& response)
{
	if (postContent(ctxt.path, request.getContent()) < 0)
		return (errorResponse(ctxt, response, Status::InternalServerError));
	response.setStatus(Status::Created);
	response.setHeader("Location", request.getUri().toString());
	if (ctxt.location.getAction() == action::cgi)
	{
		setEnvironment(request, ctxt.server, ctxt.path);
		std::string cgiExecPath = ctxt.server.findLocation(ctxt.path.c_str()).getCgiExec();
		std::pair<content_type, ft::error_code> cgiPair = getCgiResponse(cgiExecPath);
		unsetEnvironment();
		if (cgiPair.second.value() != 0)
			return errorResponse(ctxt, response, Status::InternalServerError);
		Message cgiResponse = parseCgiResponse(cgiPair.first);
		std::string cgiStatus = cgiResponse.getHeader("Status");
		int status = strtol(cgiStatus.c_str(),  NULL, 10);
		if (isError(Status(status)))
			return (errorResponse(ctxt, response, Status(status)));	
		if (postContent(ctxt.path, cgiResponse.getContent()) < 0)
			return errorResponse(ctxt, response, Status::InternalServerError);
	}
	ReturnDirective rdir;
	rdir.setUri(request.getUri().toString());
	rdir.setCode(Status::Created);
	response.setContent(html::buildRedirectionPage(rdir), "text/html");
	return response;
}

Response deleteResponse(const Context& ctxt, Response& response)
{
	if (remove(ctxt.path.c_str()) != 0)
		return errorResponse(ctxt, response, Status::NotFound);
	response.setStatus(Status::OK);
	response.setContent(html::buildSimplePage("File deleted"), "text/html");
	return response;
}

Response redirectResponse(const Context& ctxt, Response& response)
{
	const ReturnDirective& rdir = ctxt.location.getReturnDirective();
	response.setStatus(rdir.getCode());
	if (rdir.hasUri())
	{
		response.setContent(html::buildRedirectionPage(rdir), "text/html");
		response.setHeader("Location", rdir.getUri().toString());
	}
	else
		response.setContent(ft::vectorizeString(rdir.getText()), "text/plain");
	return response;
}

Response autoIndexResponse(const Context& ctxt, Request& request, Response& response)
{
	if ((*--ctxt.path.end()).empty())
	{
		response.setStatus(Status::OK);
		response.setContent(html::buildAutoindexPage(ctxt.path, request.getUri().toString()), "text/html");
	}
	else
	{
		ReturnDirective rdir;
		rdir.setUri(request.getUri().toString() + "/");
		rdir.setCode(Status::MovedPermanently);
		response.setStatus(rdir.getCode());
		response.setHeader("Location", rdir.getUri().toString());
		response.setContent(html::buildRedirectionPage(rdir), "text/html");
	}
	return response;
}

Response errorResponse(const Context& ctxt, Response& response, Status error)
{
	response.setStatus(error);
	if (error == Status::BadRequest || error == Status::PayloadTooLarge)
		response.setHeader("Connection", "close");
	fs::path errorPath = ctxt.server.getErrors()[error.getValue()];
	fs::path completePath = ServerConfig::getInstance().getPathFromUri(errorPath.c_str());
	if (!errorPath.empty() && ft::filesystem::is_regular_file(completePath))
	{
		response.setHeader("Content-Type", ServerConfig::getInstance().getMime(errorPath.extension()));
		response.setContent(getFileContent(completePath));
	}
	else
		response.setContent(html::buildErrorPage(error), "text/html");
	return response;
}

int postContent(std::string path, content_type content)
{
	int fd;
	if ((fd = open(path.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0666)) < 0)
		return -1;
	std::string scontent = ft::stringifyVector(content);
	int r = write(fd, scontent.c_str(), content.size());
	close(fd);
	return r;
}

std::vector<unsigned char> getFileContent(const fs::path& path)
{
	std::ifstream file;
	file.open(path.c_str(), std::ifstream::in);
	std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return buffer;
}

}; /* namespace http */