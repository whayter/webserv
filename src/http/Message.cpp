/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 18:55:56 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/12 22:50:03 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http.hpp"
#include "Message.hpp"
#include "Status.hpp"

namespace http {

Message::Message() {}

Message::~Message()
{
	_headers.clear();
	_content.clear();
}

std::string	Message::getHeader(std::string name)
{
	return _headers[name];
}

void Message::setStatus(Status::StatusEnum e)
{
	_status = e;
}

void Message::setHeader(std::string key, std::string val)
{
	_headers[key] = val;
}

void Message::setContent(content_type content)
{
	_content = content;
}

} /* namespace http */