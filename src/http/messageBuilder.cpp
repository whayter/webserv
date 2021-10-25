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
	Context ctxt = getContext(request.getUri());
	if (!ctxt.location.hasLimitExceptMethod(request.getMethod()))
		return errorResponse(request.getUri(), Status::MethodNotAllowed);
	if (request.getMethod() == "DELETE")
		return deleteResponse(request, ctxt.path);
	if (request.getMethod() == "POST")
		postContent(ctxt.path, request.getContent());
	if (ctxt.location.getAction() == action::cgi)
		return dynamicResponse(request, ctxt.server, ctxt.path);
	else if (ctxt.location.getAction() == action::returnDirective)
		return redirectResponse(ctxt.location.getReturnDirective());
	else if (ctxt.location.getAction() == action::none)
	{
		ft::error_code ec;
		ft::filesystem::file_status stat = ft::filesystem::status(ctxt.path, ec);
		if (ec.value() == ft::errc::no_such_file_or_directory)
			return errorResponse(request.getUri(), Status::NotFound);
		else if (ec)
			throw std::logic_error("Houston, we have a problem (" + ec.message() + ')');			
		else if (stat.type() == ft::filesystem::file_type::regular)
			return staticResponse(ctxt.path);
		else if (stat.type() == ft::filesystem::file_type::directory && ctxt.location.hasAutoindex())
			return autoIndexResponse(request, ctxt.path);
	}
	return errorResponse(request.getUri(), Status::NotFound);
}

void postContent(std::string path, content_type content)
{
	int fd = open(path.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0666);
	std::string scontent = ft::stringifyVector(content);
	int r = write(fd, scontent.c_str(), content.size());
	if (r == -1)
		std::cout << "error write(): " << strerror(errno) << std::endl;
	close(fd);
}

Response deleteResponse(Request& request, std::string path)
{
	Response result;
	if (remove(path.c_str()) != 0)
		return errorResponse(request.getUri(), Status::NotFound);
	result.setStatus(Status::OK);
	result.setContent(html::buildSimplePage("File deleted"));
	return result;
}

Response staticResponse(const ft::filesystem::path& path)
{
	Response result;
	result.setStatus(Status::OK);
	result.setContent(getFileContent(path));
	std::string extension = ServerConfig::getInstance().getMime(path.extension());
	result.setHeader("Content-Type", extension);
	return result;
}

Response dynamicResponse(http::Request& request, ServerBlock& sblock, fs::path& path)
{
	Response result;
	setEnvironment(request, sblock, path);
	std::string cgiExecPath = sblock.findLocation(path.c_str()).getCgiExec();
	std::pair<content_type, ft::error_code> cgiPair = getCgiResponse(cgiExecPath);
	unsetEnvironment();
	if (cgiPair.second.value() != 0)
	{
		std::cout << "Error cgi: " << cgiPair.second.value() << " " << cgiPair.second.message() << std::endl;
		return errorResponse(request.getUri(), 500);
	}
	Message cgiResponse = parseCgiResponse(cgiPair.first);
	std::string cgiStatus = cgiResponse.getHeader("Status");
	int status = strtol(cgiStatus.c_str(),  NULL, 10);
	if (isError(Status(status)))
		return (errorResponse(request.getUri(), Status(status)));
	if (status == 0)
		result.setStatus(Status::OK);
	else
		result.setStatus(Status(status));
	if (request.getMethod() == "POST")
	{
		result.setHeader("Content-Length", "0");
		result.setContent(content_type());
		postContent(path, cgiResponse.getContent());
	}
	else
	{
		result.setContent(cgiResponse.getContent());
		result.setHeader("Content-Type", cgiResponse.getHeader("Content-type"));	
	}
	return result;
}

Response redirectResponse(const ReturnDirective &rdir)
{
	Response result;
	result.setStatus(rdir.getCode());
	if (rdir.hasUri())
	{
		result.setContent(html::buildRedirectionPage(rdir));
		result.setHeader("Content-Type", "text/html");
		result.setHeader("Location", rdir.getUri().toString());
	}
	else
	{
		result.setContent(ft::vectorizeString(rdir.getText()));
		result.setHeader("Content-Type", "text/plain");
	}
	return result;
}

Response autoIndexResponse(http::Request& request, const ft::filesystem::path& path)
{
	Response result;

	if ((*--path.end()).empty()) // if empty then the request uri end with /
	{
		result.setStatus(Status::OK);
		result.setHeader("Content-Type", "text/html");
		result.setContent(html::buildAutoindexPage(path, request.getUri().toString()));
	}
	else // else redirect to request uri += '/'
	{
		ReturnDirective rdir;
		rdir.setUri(request.getUri().toString() + "/");
		rdir.setCode(Status::MovedPermanently);
		
		result.setStatus(Status::MovedPermanently);
		result.setHeader("Location", rdir.getUri().toString());
		result.setHeader("Content-Type", "text/html");
		result.setContent(html::buildRedirectionPage(rdir));
	}
	return result;
}

Response errorResponse(const Uri& uri, Status error)
{
	Response result;
	ServerBlock& server = ServerConfig::getInstance().findServer(uri);
	result.setStatus(error);
	if (error == Status::BadRequest)
		result.setHeader("Connection", "close");
	ft::filesystem::path path = server.getErrors()[error.getValue()];
	ft::filesystem::path completePath = ServerConfig::getInstance().getPathFromUri(path.c_str());
	if (!path.empty() && ft::filesystem::is_regular_file(completePath))
	{
		result.setHeader("Content-Type", ServerConfig::getInstance().getMime(path.extension()));
		result.setContent(getFileContent(completePath));
	}
	else
	{
		result.setHeader("Content-Type", "text/html");
		result.setContent(html::buildErrorPage(error));
	}
	return result;
}

std::vector<unsigned char> getFileContent(const ft::filesystem::path& path)
{
	std::ifstream file;
	file.open(path.c_str(), std::ifstream::in);
	std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return buffer;
}

}; /* namespace http */