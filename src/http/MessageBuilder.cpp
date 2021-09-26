/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBuilder.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 23:42:16 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/25 17:05:34 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "MessageBuilder.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "utility.hpp"
#include "Status.hpp"

#include "Location.hpp"

#include "HtmlBuilder.hpp"

#include "cgi.hpp"

#include "filesystem.hpp"
#include "parserMessage.hpp"

#include <stdio.h>
#include <fstream>

namespace fs = ft::filesystem;

namespace http {

MessageBuilder::MessageBuilder() {}

MessageBuilder::~MessageBuilder() {}


/* -------------------------------------- RESPONSE EN CHANTIER ------------------------------------ */


http::Response MessageBuilder::buildResponse(Request& request)
{
	Response response;
	ServerConfig& config = ServerConfig::getInstance();
	ServerBlock& sblock = config.findServer(request.getUri());
	fs::path path = sblock.getPathFromUri(request.getUri());
	// const Location& location = config.findLocation(sblock, path.c_str());
	const Location& location = sblock.findLocation(path.c_str());
	action aAAhhction = location.getAction();							// nom de merde lol

	if (aAAhhction == action::cgi)
		setDynamicContent(request, response, sblock, path);
	else if (aAAhhction == action::returnDirective)
		// return make_redirect();
		std::cout << "return directive" << std::endl;
	else if (aAAhhction == action::none)
	{
		// setLocalContent(config, request, response);
	
		ft::error_code ec;
		ft::filesystem::file_status stat = ft::filesystem::status(path, ec);

		if (ec.value() == ft::errc::no_such_file_or_directory)
			return make_error(request.getUri(), Status::NotFound);
		if (ec)
			throw std::logic_error("Not implemented:" + ec.message());
		if (stat.type() == ft::filesystem::file_type::regular)
			return make_static_content(path);
		else if (stat.type() == ft::filesystem::file_type::directory) // && autoindex on
			response.setContent(ft::vectorizeString(make_autoindex(path))); // changer de namespace la fonction make autoindex ? et la faire retourner un vector ou une response aussi lool XD
	}
	//return make_error(request.getUri(), Status::NotFound );


	// if (request.getMethod() == "DELETE")
	// {
	// 	if (remove(request.getUri().getPath().c_str()) == 0)
	// 		response.setStatus(Status::OK);
	// 	else
	// 		response.setStatus(Status::NoContent);
	// }
	// else if (request.getMethod() == "GET")
	// {
	// 	std::cout << "later..." << std::endl;
	// 	response.setStatus(Status::NotFound);	
	// }
	// else if (request.getMethod() == "POST")
	// {
	// 	std::cout << "later..." << std::endl;
	// }
	// else
	// 	response.setStatus(Status::MethodNotAllowed);	
	// response.setVersion("HTTP/1.1");
	// response.setHeader("Date", ft::getDate());	
	return response;
}

Response make_static_content(const ft::filesystem::path& path)
{
	Response result;

	result.setStatus(Status::OK);
	result.setContent(get_file_content(path));
	return result;
}

Response make_error(const Uri& uri, Status error)
{
	ServerBlock& server = ServerConfig::getInstance().findServer(uri);
	Response result;

	result.setStatus(error);
	ft::filesystem::path path = server.getErrors()[error.getValue()];
	if (!path.empty() && ft::filesystem::is_regular_file(path))
		result.setContent(get_file_content(path));
	else
		result.setContent(ft::vectorizeString("error " + ft::intToString(error.getValue()) + " " + error.getDefinition()));
	return result;
}

std::vector<unsigned char> 	get_file_content(const ft::filesystem::path& path)
{
	std::ifstream file;
	file.open(path.c_str(), std::ifstream::in);
	std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

	file.close();
	return buffer;
}

std::string make_autoindex(const ft::filesystem::path& path)
{
	html::Builder head = html::Builder("head");
	head.addChild("title", "Index of /webserv"); // a changer car en dur

	html::Builder body = html::Builder("body");
	html::Builder pre = html::Builder("pre");
	pre.addChild( html::Builder("a", "../").addAttribute("href", "../"));
	{
		ft::filesystem::directory_iterator it(path);
		while (it != ft::filesystem::directory_iterator())
		{
			pre.addChild(
				html::Builder("a", it->path().filename()).addAttribute("href", it->path().filename())
			);
			++it;
		}
	}
	body.addChild("h1", "Index of /webserv"); //  <- a changer car en dur... 
	body.addChild("hr","");
	body.addChild(pre);
	body.addChild("hr","");
	return html::Builder("html").addChild(head)->addChild(body)->str();	
}



void MessageBuilder::setStaticContent(ServerConfig& config, http::Request& request, http::Response &response)
{	
	std::ifstream ifs;
	std::string root = config.findServer(request.getUri()).getRoot();
	std::string path = request.getUri().getPath();
	std::string file = "." + root + path;					// root will always start with '/' ?
	ifs.open(file.c_str());
	if (!ifs)
		return setErrorContent(response, Status::NotFound);
	response.setStatus(http::Status::OK);		// set ici ?
	ifs.seekg(0, ifs.end);
	int len = ifs.tellg();
	ifs.seekg(0, ifs.beg);
	char buffer[len];
	ifs.read(buffer, len);
	std::string c = buffer;
	response.setContent(http::content_type(c.begin(), c.end()));
}



void setDynamicContent(http::Request& request, http::Response& response, ServerBlock& sblock, fs::path& path)
{
	//setEnvironment(config.findServer(request.getUri()), request);

	setEnvironment(request, sblock, path);

#ifdef LINUX
		char cgiExecPath[] = "/usr/bin/php-cgi";			// tmp
#else
		char cgiExecPath[] = "/usr/local/bin/php-cgi";		// tmp
#endif

	std::vector<unsigned char> buffer = getCgiResponse(cgiExecPath);
	unsetEnvironment();

	http::Message responseCgi = http::parseCgiResponse(buffer);
	response.setContent(responseCgi.getContent());
}

void MessageBuilder::setErrorContent(http::Response& response, Status error)
{
	response.setStatus(error);
	std::string content = "<!DOCTYPE html>";
	content += "<html lang=\"en\"><head><meta charset=\"utf-8\">";
	content += "<title>" + ft::intToString(response.getStatus().getValue());
	content += " " + response.getStatus().getDefinition() + "</title>";
	content += "<style>body{text-align:center}</style></head>";
	content += "<body><h1>" + ft::intToString(response.getStatus().getValue());
	content += + " " + response.getStatus().getDefinition() + "</h1>";
	content += "<hr size=\"3\">";
	content += "<p>Webserv</p></body></html>";
	response.setContent(http::content_type(content.begin(), content.end()));
}

}; /* namespace http */