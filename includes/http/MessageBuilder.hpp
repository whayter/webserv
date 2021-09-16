/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBuilder.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 23:38:35 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/16 14:12:57 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_BUILDER_HPP
#define MESSAGE_BUILDER_HPP

#include "http.hpp"
#include "utility.hpp"

namespace http {

class MessageBuilder {
	
public:

	MessageBuilder();
	~MessageBuilder();

	http::Request			buildRequest();
	http::Response			buildResponse(Request& request);

	void parseStatusLine();
	void parseHeaders();
	void parseContent();

	std::string				stringifyContent(content_type& content);

	template <class Message>
	std::string				stringifyMessage(Message& message)
	{
		std::string stringMessage;
		stringMessage = message.getVersion() + " ";
		stringMessage += ft::intToString(message.getStatus().getValue());
		stringMessage += " " + message.getStatus().getDefinition() + "\r\n";
		headers_type::iterator it;
		for (it = message.getHeaders().begin(); it != message.getHeaders().end(); it++)
			stringMessage += it->first + ": " + it->second + "\r\n";
		stringMessage += "\r\n" + stringifyContent(message.getContent());
		return stringMessage;
	}

}; /* class MessageBuilder */
	
}; /* namespace http */

#endif /* SUPERVISOR_HPP */