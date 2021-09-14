/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBuilder.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 23:42:16 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/14 10:51:05 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageBuilder.hpp"
#include "Message.hpp"
#include "utility.hpp"

namespace http {

MessageBuilder::MessageBuilder() {}

MessageBuilder::~MessageBuilder() {}

std::string	MessageBuilder::stringifyContent(content_type& content)
{
	return std::string(content.begin(), content.end());
}

std::string MessageBuilder::stringifyMessage(Message& message)
{
	std::string stringMessage;
	headers_type headers;
	headers_type::iterator it;
	stringMessage = "http/1.1 " + intToString(message.getStatus().getValue());
	stringMessage += ' ' + message.getStatus().getDefinition() + "\r\n";
	for (it = headers.begin(); it != headers.end(); it++)
		stringMessage += it->first + ":\t" + it->second + "\r\n";
	content_type content = message.getContent();
	stringMessage += "\r\n" + stringifyContent(content) + "\r\n";
	return stringMessage;
}

}; /* namespace http */