/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpMessage.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 13:20:58 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/08 14:34:07 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/AHttpMessage.hpp"

AHttpMessage::AHttpMessage() {}
AHttpMessage::~AHttpMessage() {}

std::string AHttpMessage::getContent()
{
	return _content;
}

// AHttpMessage::size_type AHttpMessage::getContentLength()
// {
// 	size_type result;
// 	std::istringstream(getHeader("Content-Length")) >> result;
// 	return result;
// }

AHttpMessage::map_type AHttpMessage::getHeaders()
{
	return _headers;
}

std::string AHttpMessage::getHeader(std::string name)
{
	return _headers.at(name);
}

void AHttpMessage::readContent(std::istream is)
{
	is >> _content;
}

void AHttpMessage::setContent(std::string content)
{
	// this->setContentLength(content.size());
	_content = content;
}

// void AHttpMessage::setContentType(std::string type)
// {
// 	this->addHeader("Content-Type", type);       
// }

void AHttpMessage::addHeader(std::string name, std::string value)
{
	_headers[name] = value;
}

// void AHttpMessage::setHeaders(map_type headers)
// {
// 	_headers = headers;
// }