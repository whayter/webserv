/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBuilder.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 23:42:16 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/18 12:06:14 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "MessageBuilder.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "utility.hpp"
#include "Status.hpp"

#include "cgi.hpp"

#include "filesystem.hpp"

#include <stdio.h>
#include <fstream>

namespace http {

MessageBuilder::MessageBuilder() {}

MessageBuilder::~MessageBuilder() {}


/* -------------------------------------- RESPONSE EN CHANTIER ------------------------------------ */

http::Response MessageBuilder::buildResponse(Request& request)
{
	ServerConfig& config = ServerConfig::getInstance();
	Response response;

	// findLocation here
	// if location.getCgiExec() call cgi
	// else do something
	
	/* tmp tmp tmp tmp tmp tmp tmp tmp tmp tmp tmp tmp tmp */
	std::string path = request.getUri().getPath();
	size_t pos = path.find_last_of('.') + 1;
	if (path.substr(pos) == "php")
		setCgiContent(request, response, config);
	else
		setLocalContent(config, request, response);
	/* tmp tmp tmp tmp tmp tmp tmp tmp tmp tmp tmp tmp tmp */

	
	response.setHeader("Content-Length", ft::intToString(response.getContent().size()));
	
	response.setHeader("Content-Type", "text/html"); // tmp
	
	response.setVersion("HTTP/1.1");
	response.setHeader("Server", "Webserv");
	response.setHeader("Date", ft::getDate());


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



void MessageBuilder::setLocalContent(ServerConfig& config, http::Request& request, http::Response &response)
{
	std::ifstream ifs;
	std::string root = config.findServer(request.getUri()).getRoot();
	std::string path = request.getUri().getPath();
	std::string file = "." + root + path;					// root will always start with '/' ?
	ifs.open(file.c_str());
	if (!ifs)
	{
		response.setStatus(http::Status::NotFound);
		setErrorContent(response);
		return ;
	}
	response.setStatus(http::Status::OK);		// set ici ?
	ifs.seekg(0, ifs.end);
	int len = ifs.tellg();
	ifs.seekg(0, ifs.beg);
	char buffer[len];
	ifs.read(buffer, len);
	std::string c = buffer;
	response.setContent(http::content_type(c.begin(), c.end()));
}

void MessageBuilder::setCgiContent(http::Request& request, http::Response& response, ServerConfig& config)
{
	std::vector<unsigned char> cgiHeaders;
	std::vector<unsigned char> cgiContent;
	setEnvironment(config.findServer(request.getUri()), request);
	callCgi(&cgiHeaders, &cgiContent);
	unsetEnvironment();
	parseCgiHeaders(cgiHeaders, response);
	response.setContent(cgiContent);
}

void MessageBuilder::parseCgiHeaders(std::vector<unsigned char>& cgiHeaders, http::Response& response)
{
	std::stringstream ss(std::string(cgiHeaders.begin(), cgiHeaders.end()));
	size_t n = std::count(cgiHeaders.begin(), cgiHeaders.end(), '\r');
	for (size_t i = 0; i < n - 1; i++)
	{
		std::string line;
		getline(ss, line);
		int firstPos = line.find(':');
		std::string name = line.substr(0, firstPos);
		int secondPos = line.find('\r');
		std::string value = line.substr(firstPos + 2, secondPos);
		if (name == "Status")
			response.setStatus(ft::stringifyInteger(value.substr(0, value.find_first_of(' '))));
		else if (name == "Content-type")
			response.setHeader(name, value);
	}
}

void MessageBuilder::setErrorContent(http::Response& response)			// not complete yet
{
	std::string content = "<!DOCTYPE html>";
	content += "<html lang=\"en\"><head><meta charset=\"utf-8\">";
	content += "<title>" + ft::intToString(response.getStatus().getValue());
	content += " " + response.getStatus().getDefinition() + "</title>";
	content += "<style>body{text-align:center}</style></head>";
	content += "<body><h1>" + ft::intToString(response.getStatus().getValue());
	content += + " " + response.getStatus().getDefinition() + "</h1>";
	content += "<hr size=\"3\">";
	if (http::isRedirection(response.getStatus()))
	{
		content += "<p>The document has moved ";
		content += "<a href=\"\">here</a></p>";				// add new uri in href to create a link
	}
	else
		content += "<p>Webserv</p></body></html>";
	response.setContent(http::content_type(content.begin(), content.end()));
}

}; /* namespace http */