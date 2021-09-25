/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBuilder.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 23:42:16 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/24 18:07:59 by hwinston         ###   ########.fr       */
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

namespace http {

MessageBuilder::MessageBuilder() {}

MessageBuilder::~MessageBuilder() {}


/* -------------------------------------- RESPONSE EN CHANTIER ------------------------------------ */

http::Response MessageBuilder::buildResponse(Request& request)
{
	Response response;
	ServerConfig& config = ServerConfig::getInstance();
	ft::filesystem::path path = config.findServer(request.getUri()).getPathFromUri(request.getUri());
	Location& location = config.findLocation(path.c_str()); // bhrr, chelou mais sinon c'est pas la bonne location lol
	// par exemple, GET localhost:80 sera rendu en statique au lieu de cgi. 

	// le path final. Avec le bon root, et index concatene automatiquement si necessaire.
	// Et peu importe si cgi ou statique. Peut etre fichier ou dossier. 
	// bon par contre effectivement, le nom de la fonction
	// est nul a chier lol "getPathFromUri". J'ai pas trouve mieux lol
	// mais ca bug je crois. par exemple, localhost retourne un 404 au lieu de retourner index.php... XD
	action a = location.getAction();
	if (a == action::cgi)
	{
		std::cout << "calling cgi !" << std::endl;
		setCgiContent(request, response, config);
	}
	else if (a == action::returnDirective)
	{
		// return make_redirect();
	}
	else if (a == action::none)
	{
		std::cout << "static !" << std::endl;
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
			response.setContent(ft::vectorizeString(html::make_autoindex(path))); // changer de namespace la fonction make autoindex ? et la faire retourner un vector ou une response aussi lool XD
	}
	return make_error(request.getUri(), Status::NotFound );

	
	/* tmp tmp tmp tmp tmp tmp tmp tmp tmp tmp tmp tmp tmp */
	// size_t pos = p.string().find_last_of('.') + 1;
	// if (p.string().substr(pos) == "php")
	// 	setCgiContent(request, response, config);
	// else
	// 	setLocalContent(config, request, response);
	/* tmp tmp tmp tmp tmp tmp tmp tmp tmp tmp tmp tmp tmp */

	
	// response.setHeader("Content-Length", ft::intToString(response.getContent().size()));
	
	// response.setHeader("Content-Type", "text/html"); // tmp
	
	// response.setVersion("HTTP/1.1");
	// response.setHeader("Server", "Webserv");
	// response.setHeader("Date", ft::getDate());


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
	return buffer;
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
	// parseCgiHeaders(cgiHeaders, response);
	// response.setContent(cgiContent);
// deso ugly fix
	http::Message responseCgi = http::parseCgiResponse(cgiHeaders);
	response.setContent(responseCgi.getContent());
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