/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 14:47:59 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/12 22:22:44 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HttpResponse.hpp"

HttpResponse::HttpResponse()
{
	_status.setValue(HttpStatus::OK);
}

HttpResponse::~HttpResponse() {}

int HttpResponse::getStatus()
{
	return _status.getValue();
}

std::string HttpResponse::getStatusMessage(int code)
{
	return _status.getMessage(code);
}

void HttpResponse::setStatus(int code)
{
	_status.setValue(code);
}

void HttpResponse::setContentLength(int contentLength)
{
	std::ostringstream os;
	os << contentLength;
	this->addHeader("Content-Length", os.str());
}

void HttpResponse::read(std::istream is)
{
	std::string line, s;
	size_t pos;
	
	try
	{
		std::getline(is, line);
		pos = line.find(" ");



		while (!is.eof())
		{
			std::getline(is, line);
			pos = line.find(":");
			s = line.substr(0, pos);
			line.erase(0, pos + 1);
			this->addHeader(s, line);
		}
	}
	catch(const std::exception& e)
	{
		this->setStatus(HttpStatus::NoContent);
	}
}

void HttpResponse::sendError(int code, std::ofstream out)
{
	out << "<html><body><p>HTTP Error ";
	out << code << " - " << this->getStatusMessage(code);
	out << "</p></body></html>";
	setStatus(code);
}

std::string HttpResponse::toString()
{
	std::string s;
	std::ostringstream oss;
	map_type header;
	map_type::iterator it;

	s = "Status:\n\t";
	oss << getStatus();
	s += oss.str() + "\n";
	s += "Headers:\n";
	header = this->getHeaders();
	for (it = header.begin(); it != header.end(); it++)
		s += "\t" + it->first + "\t" + it->second + "\n";
	return s;
}

void HttpResponse::write(std::ostream os)
{
	map_type header;
	map_type::iterator it;

	setContentLength(_content.length());
	os << "HTTP/1.1 ";
	os << this->getStatus() << " ";
	os << this->getStatusMessage(this->getStatus()) << "\n";
	header = this->getHeaders();
	for (it = header.begin(); it != header.end(); it++)
		os << it->first << ": " << it->second << "\n";
	os << _content;
	os.flush();
}
