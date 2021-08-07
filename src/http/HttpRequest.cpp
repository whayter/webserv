/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 14:36:30 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/07 11:43:39 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HttpRequest.hpp"

HttpRequest::HttpRequest(): AHttpMessage() {}

HttpRequest::~HttpRequest() {}

std::string HttpRequest::getMethod()
{
	return _method;
}

std::string HttpRequest::getRequestURI()
{
	if (_uri.getPath().empty())
		return "index.html";
	else
		return _uri.getPath();
}

// void HttpRequest::read(std::istream is)
// {
// 	std::string line, request, s;
// 	size_type pos;

// 	std::getline(is, line);
// 	std::stringstream ss(line);
// 	std::getline(ss, _method, ' ');
// 	std::getline(ss, request, ' ');
// 	if (!ss.eof())
// 		std::getline(ss, _version);
// 	else
// 		_version = "HTTP/0.9";
// 	_uri = Uri(request);
// 	_queryString = _uri.getQueryString();
// 	_queryParameters = _uri.getQueries(_queryString);
// 	while (!is.eof())
// 	{
// 		std::getline(is, line);
// 		pos = line.find(":");
// 		s = line.substr(0, pos);
// 		line.erase(0, pos + 1);
// 		this->setHeader(s, line);
// 	}
// 	this->setContentLength(this->getContentLength());
// }

void HttpRequest::setMethod(std::string method)
{
	_method = method;
}

void HttpRequest::setUri(const Uri &uri)
{
	_uri = uri;
}

void HttpRequest::setVersion(std::string version)
{
	_version = version;
}

std::string HttpRequest::toString()
{
	std::string s;
	map_type header;
	map_type::iterator it;

	s = "Method:\n\t" + getMethod() + "\n";
	s += "URI:\n\t" + getRequestURI() + "\n";
	s += "Parameters:\n\t" + _queryString + "\n";
	s += "Headers:\n";
	header = this->getHeaders();
	for (it = header.begin(); it != header.end(); it++)
		s += "\t" + it->first + "\t" + it->second + "\n";
	return s;
}

void HttpRequest::write(std::ostream os)
{
	map_type header;
	map_type::iterator it;

	os << _method << " ";
	os << this->getRequestURI() << " ";
	os << _version << "\n";
	for (it = header.begin(); it != header.end(); it++)
		os << it->first << ": " << it->second + "\n";
	os.flush();
}