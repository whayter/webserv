/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 18:55:56 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/12 18:56:35 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "Message.hpp"

http::Message::Message() {}

http::Message::~Message() {}

int					http::Message::getStatusCode()
{
	return _statusCode;
}

std::string			http::Message::getStatusMessage() const
{
	return http::Status::getDefinition(_statusCode);
}

http::headers_type	http::Message::getHeaders() const
{
	return _headers;
}

http::content_type	http::Message::getContent() const
{
	return _content;
}

std::string			http::Message::getHeader(std::string name)
{
	return _headers[name];
}

void				http::Message::setStatusCode(int code)
{
	_statusCode = code;
}

void				http::Message::setHeader(std::string key, std::string val)
{
	_headers[key] = val;
}

void				http::Message::setContent(content_type content)
{
	_content = content;
}