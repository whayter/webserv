/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 14:47:59 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/02 20:30:46 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "ServerConfig.hpp"
#include "utility.hpp"

/* --- Public functions ----------------------------------------------------- */

HttpResponse::HttpResponse(ServerBlock serverBlock, HttpRequest& request)
: _serverBlock(serverBlock), _request(request)
{
	int code = request.getHttpErrorCode();
	if (code == 0)
		_setStatus(HttpStatus::OK);
	else
		_setStatus(code);
}

HttpResponse::~HttpResponse() {}


void HttpResponse::setLocalContent()
{
	std::ifstream ifs;
	std::string root = _serverBlock.getRoot();
	std::string path = _request.getUri().getPath();
	std::string file = "." + root + path;					// root will always start with '/' ?
	ifs.open(path.c_str());
	if (!ifs)
	{
		_code.setValue(HttpStatus::NotFound);
		setErrorContent();
		return ;
	}
	ifs.seekg(0, ifs.end);
	int len = ifs.tellg();
	ifs.seekg(0, ifs.beg);
	char buffer[len];
	ifs.read(buffer, len);
	_content = buffer;
}

void HttpResponse::setErrorContent()												// not complete yet
{
	_content = "<!DOCTYPE html>";
	_content += "<html lang=\"en\"><head><meta charset=\"utf-8\">";
	_content += "<title>" + intToString(_getStatus());
	_content += " " + _getStatusMessage() + "</title>";
	_content += "<style>body{text-align:center}</style></head>";
	_content += "<body><h1>" + intToString(_getStatus());
	_content += + " " + _getStatusMessage() + "</h1>";
	_content += "<hr size=\"3\">";
	if (_getStatus() == HttpStatus::MovedPermanently)
	{
		_content += "<p>The document has moved ";
		//_content ++ "<a href=\"" + new_location + "\">here</a></p>";				// a ajouter
	}
	else
		_content += "<p>Webserv</p></body></html>";
}

void HttpResponse::setMandatory()
{
	_setStatusLine();
	_setDate();
	_setServer();

	setLocalContent();			// for local get

	_setContentLength();

	_setContentType(_request.getUri());
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
	this->addHeader("Date", getDate());
}

void 		HttpResponse::_setServer(void)
{
	this->addHeader("Server", "Webserv");
}

void 		HttpResponse::_setContentLength(void)
{
	this->addHeader("Content-Length", intToString(_content.size()));
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
