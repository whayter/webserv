/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessage.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 18:55:56 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/15 16:20:58 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMessage.hpp"
#include "utility.hpp"
#include <cstdlib>

namespace http {

AMessage::AMessage() {}

std::string	AMessage::getHeader(std::string name)
{
	return _headers[name];
}

void AMessage::setHeader(std::string key, std::string val)
{
	_headers[key] = val;
}

void AMessage::setContent(const content_type &content)
{
	_content = content;
	if (getHeaders().find("Content-Length") != getHeaders().end())
		this->setHeader("Content-Length", ft::intToString(content.size()));
}

void AMessage::setVersion(const std::string &version)
{
	_version = version;
}

size_t AMessage::getContentLength()
{
	const char*			key = "Content-Length";
	http::headers_type	headers = getHeaders();

	if (headers.find(key) != headers.end())
		return ::strtoul(headers[key].c_str(), 0, 10);	
	return 0;
}

void AMessage::setVersion(std::string version)
{
	_version = version;
}

} /* namespace http */