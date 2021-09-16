/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBuilder.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 23:42:16 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/15 17:14:28 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageBuilder.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "utility.hpp"

#include "filesystem.hpp"

namespace http {

MessageBuilder::MessageBuilder() {}

MessageBuilder::~MessageBuilder() {}

http::Response MessageBuilder::buildResponse(Request& request)
{
	Response response;

	std::cout << "METHOD = " << request.getMethod() << std::endl;

	if (request.getMethod() == "DELETE")
	{
		if (remove(request.getUri().getPath().c_str()) == 0)
			response.setStatus(Status::OK);
		else
			response.setStatus(Status::NoContent);
	}
	else
	{
		std::cout << "later..." << std::endl;
	}
	response.setVersion("HTTP/1.1");
	response.setHeader("Date", ft::getDate());

	// else
	// 	response.setStatus(Status::MethodeNotAllowed);
	
	return response;
}

std::string	MessageBuilder::stringifyContent(content_type& content)
{
	return std::string(content.begin(), content.end());
}

// template <class Message>
// std::string MessageBuilder::stringifyMessage(Message& message)
// {
// 	std::string stringMessage;
// 	headers_type headers;
// 	headers_type::iterator it;
// 	stringMessage = message.getVersion() + intToString(message.getStatus().getValue());
// 	stringMessage += ' ' + message.getStatus().getDefinition() + "\r\n";
// 	for (it = headers.begin(); it != headers.end(); it++)
// 		stringMessage += it->first + ":\t" + it->second + "\r\n";
// 	content_type content = message.getContent();
// 	stringMessage += "\r\n" + stringifyContent(content) + "\r\n";
// 	return stringMessage;
// }

}; /* namespace http */