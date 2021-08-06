/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpMessage.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 13:20:58 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/04 14:52:16 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/AHttpMessage.hpp"

AHttpMessage::AHttpMessage() {}

std::string AHttpMessage::getContent()
{
	return _content;
}

AHttpMessage::size_type AHttpMessage::getContentLength()
{
	size_type result;
	std::istringstream(getHeaderAt("Content-Length")) >> result;
	return result;
}

AHttpMessage::map_type AHttpMessage::getHeaders()
{
	return _headers;
}

std::string AHttpMessage::getHeaderAt(std::string name)
{
	return _headers.at(name);
}

void AHttpMessage::readContent(std::istream is)
{
	is >> _content;
}

void AHttpMessage::setContent(std::string content)
{
	this->setContentLength(content.size());
	_content = content;
}

void AHttpMessage::setContentLength(int contentLength)
{
	std::ostringstream os;
	os << contentLength;
	this->setHeader("Content-Length", os.str());
}

void AHttpMessage::setContentType(std::string type)
{
	this->setHeader("Content-Type", type);       
}

void AHttpMessage::setHeader(std::string name, std::string value)
{
	_headers.insert(std::make_pair<std::string, std::string>(name, value));
}

void AHttpMessage::setHeaders(map_type headers)
{
	_headers = headers;
}