/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 14:47:59 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/29 16:58:55 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "ServerConfig.hpp"
#include "utility.hpp"

#include <ctime>

/* --- Public functions ----------------------------------------------------- */

HttpResponse::HttpResponse(server::Server server, HttpRequest& request)
: _server(server), _request(request)
{
	int code = request.getHttpErrorCode();
	if (code == 0)
		_setStatus(HttpStatus::OK);
	else
		_setStatus(code);
}

HttpResponse::~HttpResponse() {}

void HttpResponse::setContent()
{
	// call cgi here
	methodGet("www/index.php"); 		// tmp
}

void HttpResponse::setError()
{
	_content = "<!DOCTYPE html>";
	_content += "<meta charset=\"utf-8\">";
	_content += "<html lang=\"en\"><head><title>";
	_content += intToString(_getStatus()) + "</title>";
	_content += "<style>body{text-align:center}</style></head>";
	_content += "<body><h1>" + intToString(_getStatus());
	_content += + " - " + _getStatusMessage() + "</h1></body>";
	_content += "</html>";
}

void HttpResponse::setMandatory()
{
	_setStatusLine();
	_setDate();
	_setServer();

	setContent();	// tmp

	_setContentLength(_content.size());

	_setContentType(_request.getUri());
}



void HttpResponse::methodGet(std::string filename)
{
	std::ifstream ifs;
	ifs.open(filename.c_str());
	if (!ifs)
		return ;
	ifs.seekg(0, ifs.end);
	int len = ifs.tellg();
	ifs.seekg(0, ifs.beg);
	char buffer[len];
	ifs.read(buffer, len);
	_content = buffer;
	//std::cout << _content << std::endl;
}


std::string HttpResponse::toString()
{
	std::string s;
	s = _statusLine + "\n";
	map_type::iterator header;
	for (header = _headers.begin(); header != _headers.end(); header++)
		s += header->first + ": " + header->second + "\n";
	s += "\n" + _content;
	return s;
}

/* --- Private functions ---------------------------------------------------- */

std::string	HttpResponse::_getHeader(std::string key)
{
	map_type::iterator header = _headers.find(key);
	return header->first + ": " + header->second;	
}

int			HttpResponse::_getStatus(void)
{
	return _code.getValue();
}

std::string	HttpResponse::_getStatusMessage(void)
{
	return _code.getMessage(_getStatus());
}

void		HttpResponse::_setStatus(int code)
{
	_code.setValue(code);
}

void		HttpResponse::_setStatusLine(void)
{
	_statusLine = "HTTP/1.1 ";
	std::ostringstream oss;
	oss << _getStatus();
	_statusLine += oss.str();
	_statusLine += " ";
	_statusLine += _getStatusMessage();
}

void		HttpResponse::_setDate(void)
{
	char date[30];
	time_t now = time(0);
	struct tm* lt = gmtime(&now);
	strftime(date, sizeof(date), "%a, %d %b %Y %k:%M:%S GMT", lt);
	this->addHeader("Date", date);
}

void 		HttpResponse::_setServer(void)
{
	this->addHeader("Server", _server.block.getServerName());
}

void 		HttpResponse::_setContentLength(int contentLength)
{
	std::ostringstream os;
	os << contentLength;
	this->addHeader("Content-Length", os.str());
}

void 		HttpResponse::_setContentType(const Uri& uri)
{
	(void)uri;
	addHeader("Content-Type", "text/html"); // tmp

	// scrapper le content-type dans le content demandé / retourné par cgi ?

	// std::string path = uri.getPathEtc();
	// if (path == "/")
	// {
	// 	path = _server.block.findLocation(uri).getIndex();
	// 	if (path.empty())
	// 		path = _server.block.getIndex();
	// }
	// int pos = path.find_last_of(".");
	// if (!path.at(pos) || !path.at(pos + 1))
	// {
	// 	addHeader("Content-Type", "text/plain");
	// 	return ;
	// }
	// std::string type = path.substr(pos + 1);
	// if (type == "html" || type == "htm" || type == "php")
	// 	addHeader("Content-Type", "text/html");
}
