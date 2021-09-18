/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 14:47:59 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/18 11:57:24 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "ServerConfig.hpp"
#include "utility.hpp"
#include "cgi.hpp"

#include <algorithm>

/* --- Public functions ----------------------------------------------------- */

HttpResponse::HttpResponse(ServerBlock serverBlock, HttpRequest& request)
: _serverBlock(serverBlock), _request(request)
{
	int code = request.getHttpErrorCode();
	if (code != 0)
		_setStatus(code);
}

HttpResponse::~HttpResponse() {}

void HttpResponse::setLocalContent()
{
	std::ifstream ifs;
	std::string root = _serverBlock.getRoot();
	std::string path = _request.getUri().getPath();
	std::string file = "." + root + path;					// root will always start with '/' ?
	ifs.open(file.c_str());
	if (!ifs)
	{
		_code.setValue(http::Status::NotFound);
		setErrorContent();
		return ;
	}
	_code.setValue(http::Status::OK);		// set ici ?
	ifs.seekg(0, ifs.end);
	int len = ifs.tellg();
	ifs.seekg(0, ifs.beg);
	char buffer[len];
	ifs.read(buffer, len);
	_content = buffer;
}

void HttpResponse::setCgiContent()
{
	std::vector<unsigned char> cgiHeaders;
	std::vector<unsigned char> cgiContent;
	setEnvironment(_serverBlock, _request);
	callCgi(&cgiHeaders, &cgiContent);
	unsetEnvironment();
	_parseCgiHeaders(cgiHeaders);
	_content = std::string(cgiContent.begin(), cgiContent.end());
}

void HttpResponse::setErrorContent()												// not complete yet
{
	_content = "<!DOCTYPE html>";
	_content += "<html lang=\"en\"><head><meta charset=\"utf-8\">";
	_content += "<title>" + ft::intToString(_getStatus());
	_content += " " + _getStatusMessage() + "</title>";
	_content += "<style>body{text-align:center}</style></head>";
	_content += "<body><h1>" + ft::intToString(_getStatus());
	_content += + " " + _getStatusMessage() + "</h1>";
	_content += "<hr size=\"3\">";
	if (_getStatus() == http::Status::MovedPermanently)
	{
		_content += "<p>The document has moved ";
		//_content ++ "<a href=\"" + new_location + "\">here</a></p>";				// a ajouter
	}
	else
		_content += "<p>Webserv</p></body></html>";
}

void HttpResponse::build()
{
	/* PSEUDO-CODE */
	
	// if (cgi)
	// 	callCgi();
	// else if (GET)
	// 	setLocalContent();
	// else if (POST)
	// 	post();
	// else if (DELETE)
	// 	delete();
	// else
	// 	err(not a valid method)
	// setHeaders();

	std::string path = _request.getUri().getPath();			// tmp
	size_t pos = path.find_last_of('.') + 1;				// tmp
	if (path.substr(pos) == "php")
		setCgiContent();									// tmp
	else
		setLocalContent();
	_setStatusLine();
	_setDate();
	_setServer();
	_setContentLength();
	_setContentType(_request.getUri());
}

std::string HttpResponse::toString()
{
	std::string s;
	s = _statusLine + "\r\n";
	http::headers_type headers(getHeaders());
	http::headers_type::iterator header;
	for (header = headers.begin(); header != headers.end(); header++)
		s += header->first + ": " + header->second + "\n";
	s += "\r\n" + _content;
	return s;
}

/* --- Private functions ---------------------------------------------------- */

void		HttpResponse::_parseCgiHeaders(std::vector<unsigned char>& cgiHeaders)
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
		{
			std::cout << "name = status !" << std::endl;
			_setStatus(ft::stringifyInteger(value.substr(0, value.find_first_of(' '))));
			_setStatusLine();
		}
		else if (name == "Content-type")
			setHeader(name, value);
	}
}

std::string	HttpResponse::_getHeader(std::string key)
{
	http::headers_type headers(getHeaders());
	http::headers_type::iterator header = headers.find(key);
	return header->first + ": " + header->second;	
}

int			HttpResponse::_getStatus(void)
{
	return _code.getValue();
}

std::string	HttpResponse::_getStatusMessage(void)
{
	return _code.getDefinition();
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
	this->setHeader("Date", ft::getDate());
}

void 		HttpResponse::_setServer(void)
{
	this->setHeader("Server", "Webserv");
}

void 		HttpResponse::_setContentLength(void)
{
	this->setHeader("Content-Length", ft::intToString(_content.size()));
}

void 		HttpResponse::_setContentType(const Uri& uri)
{
	(void)uri;
	setHeader("Content-Type", "text/html"); // tmp

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
	// 	setHeader("Content-Type", "text/plain");
	// 	return ;
	// }
	// std::string type = path.substr(pos + 1);
	// if (type == "html" || type == "htm" || type == "php")
	// 	setHeader("Content-Type", "text/html");
}
