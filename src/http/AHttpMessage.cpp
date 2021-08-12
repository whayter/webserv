/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpMessage.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 13:20:58 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/12 23:32:14 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/AHttpMessage.hpp"

AHttpMessage::AHttpMessage() {}
AHttpMessage::~AHttpMessage() {}

std::string AHttpMessage::getContent()
{
	return _content;
}

size_t AHttpMessage::getContentLength()
{
	return _content.size();
}

AHttpMessage::map_type AHttpMessage::getHeaders()
{
	return _headers;
}

std::string AHttpMessage::getHeader(std::string name)
{
	return _headers[name];
}

void AHttpMessage::readContent(std::istream is)
{
	is >> _content;
}

void AHttpMessage::setContent(std::string content)
{
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