/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBuilder.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 23:38:35 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/14 10:46:22 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_BUILDER_HPP
#define MESSAGE_BUILDER_HPP

#include "http.hpp"

namespace http {

class MessageBuilder {
	
public:

	MessageBuilder();
	~MessageBuilder();

	http::Message			buildRequest();
	http::Message			buildResponse();

	void parseStatusLine();
	void parseHeaders();
	void parseContent();

	std::string				stringifyContent(content_type& content);
	std::string				stringifyMessage(Message& message);

}; /* class MessageBuilder */
	
}; /* namespace http */

#endif /* SUPERVISOR_HPP */