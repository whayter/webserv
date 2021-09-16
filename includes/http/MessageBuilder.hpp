/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBuilder.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 23:38:35 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/15 17:14:55 by hwinston         ###   ########.fr       */
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
		headers_type headers;
		headers_type::iterator it;
		stringMessage = message.getVersion() + " " + ft::intToString(message.getStatus().getValue());
		stringMessage += ' ' + message.getStatus().getDefinition() + "\r\n";
		for (it = headers.begin(); it != headers.end(); it++)
			stringMessage += it->first + ":\t" + it->second + "\r\n";
		content_type content = message.getContent();
		stringMessage += "\r\n" + stringifyContent(content) + "\r\n";
		return stringMessage;
	}

}; /* class MessageBuilder */
	
}; /* namespace http */

#endif /* SUPERVISOR_HPP */