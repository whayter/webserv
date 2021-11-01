#include "ServerConfig.hpp"
#include "Context.hpp"
#include "Response.hpp"
#include "Status.hpp"
#include "messageParser.hpp"
#include "messageBuilder.hpp"
#include "cgi.hpp"
#include "utility.hpp"
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
	if (ctxt.location.getAction() == action::cgi)
		return dynamicResponse(ctxt, request, response);
	if (ctxt.location.getAction() == action::returnDirective)
		return redirectResponse(ctxt, response);
	if (ctxt.location.getAction() == action::none)
	{
		if (request.getMethod() == "GET")
			return getMethodResponse(ctxt, request, response);
		if (request.getMethod() == "POST")
			return postMethodResponse(ctxt, request, response);
		if (request.getMethod() == "DELETE")
			return deleteMethodResponse(ctxt, response);
	}
	return errorResponse(ctxt, response, Status::NotFound);
}

Response getMethodResponse(const Context& ctxt, Request& request, Response& response)
{
	ft::error_code ec;
	fs::file_status stat = fs::status(ctxt.path, ec);
	if (ec.value() == ft::errc::no_such_file_or_directory)
		return errorResponse(ctxt, response, Status::NotFound);
	if (ec.value() ==  ft::errc::filename_too_long)
		return errorResponse(ctxt, response, Status::URITooLong);
	if (ec)
		throw std::logic_error("Houston, we have a problem (" + ec.message() + ')');			
	if (stat.type() == fs::file_type::regular)
		return staticResponse(ctxt, response);
	if (stat.type() == fs::file_type::directory && ctxt.location.hasAutoindex())
		return autoIndexResponse(ctxt, request, response);
	return errorResponse(ctxt, response, Status::NotFound);
}

Response postMethodResponse(const Context& ctxt, Request& request, Response& response)
{
	if (ctxt.server.getUploadStore().empty()
	|| !ft::filesystem::is_directory(ctxt.server.getUploadStore()))
		return errorResponse(ctxt, response, Status::Forbidden);

	std::vector<std::string> vec = ft::split(request.getHeader("Content-Type"), ';');
	if (ft::trim(vec[0]) == "multipart/form-data")
		return postMultipart(ctxt, request, response);
	

	fs::path p = ctxt.server.getUploadStore() / ctxt.path.filename();
	
	if (postContent(p, request.getContent()) < 0)
		return (errorResponse(ctxt, response, Status::InternalServerError));
	response.setStatus(Status::Created);
	response.setHeader("Location", request.getUri().toString());
	ReturnDirective rdir;
	rdir.setUri(request.getUri().toString());
	rdir.setCode(Status::Created);
	response.setContent(html::buildRedirectionPage(rdir), "text/html");
	return response;
}

Response deleteMethodResponse(const Context& ctxt, Response& response)
{
	if (remove(ctxt.path.c_str()) != 0)
		return errorResponse(ctxt, response, Status::NotFound);
	response.setStatus(Status::OK);
	response.setContent(html::buildSimplePage("File deleted"), "text/html");
	return response;
}

Response staticResponse(const Context& ctxt, Response& response)
{
	std::string extension = ServerConfig::getInstance().getMime(ctxt.path.extension().c_str());
	response.setStatus(Status::OK);
	response.setContent(ft::getFileContent(ctxt.path), extension);
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
	{
		response.setContent(ft::vectorizeString(rdir.getText()), "text/plain");
		if (response.getStatus() == Status::NoContent)
		{
			response.delHeader("Content-Length");
			response.delHeader("Content-Type");
		}
	}
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
	if (error == Status::BadRequest || error == Status::PayloadTooLarge || error == Status::NotImplemented
	|| error == Status::RequestHeaderFieldsTooLarge)
		response.setHeader("Connection", "close");
	fs::path errorPath = ctxt.server.getErrors()[error.getValue()];
	fs::path completePath = ServerConfig::getInstance().getPathFromUri(errorPath.c_str());
	if (!errorPath.empty() && ft::filesystem::is_regular_file(completePath))
	{
		response.setHeader("Content-Type", ServerConfig::getInstance().getMime(errorPath.extension()));
		response.setContent(ft::getFileContent(completePath));
	}
	else
		response.setContent(html::buildErrorPage(error), "text/html");
	return response;
}

Response postMultipart(const Context& ctxt, Request& request, Response& response)
{
	std::vector<std::string> vec = ft::split(request.getHeader("Content-Type"), ';');
	std::string contentType = vec.size() > 0 ? ft::trim(vec[0]) :  "";
	if (contentType != "multipart/form-data" || vec.size() != 2)
		return errorResponse(ctxt, response, Status::NotImplemented);
	std::string boundary = ft::trim(vec[1]);
	
	vec = ft::split(boundary, '=');
	if (vec.size() != 2 || vec[0] != "boundary")
		return errorResponse(ctxt, response, Status::BadRequest); // bad request ???
	boundary = vec[1];
	
	std::vector<multipart_part> parts;
	http::Status error = parseContentMultipart(parts, request, boundary);
	if (error != Status::None)
		return errorResponse(ctxt, response, error);
	std::vector<multipart_part>::iterator it = parts.begin();
	while (it != parts.end())
	{
		std::string filename = it->getFilename();
		if (filename.empty())
			return errorResponse(ctxt, response, Status::ImATeapot);
		ft::writeContentToFile(ctxt.server.getUploadStore() / filename, reinterpret_cast<char*>(it->content), it->len);
		++it;
	}
	response.setStatus(http::Status::Created);
	response.setContent(html::buildErrorPage(http::Status::Created));
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

}; /* namespace http */