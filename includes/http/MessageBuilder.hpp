/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBuilder.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 23:38:35 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/30 19:13:52 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_BUILDER_HPP
#define MESSAGE_BUILDER_HPP

#include "http.hpp"
#include "utility.hpp"
#include "ServerConfig.hpp"
#include "pageBuilding.hpp"

namespace http {

class MessageBuilder {
	
public:

	MessageBuilder();
	~MessageBuilder();

	http::Request			buildRequest();


	http::Response			buildResponse(Request& request);


	
	void					setStaticContent(ServerConfig& config, http::Request& request, http::Response &response);
	void					setErrorContent(http::Response& response, Status error);

	void parseStatusLine();
	void parseHeaders();
	void parseContent();


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
		stringMessage += "\r\n" + ft::stringifyVector(message.getContent());
		return stringMessage;
	}

}; /* class MessageBuilder */

Response					errorResponse(const Uri& uri, Status error);
Response					redirectResponse(const ReturnDirective &rdir);
Response					staticResponse(const ft::filesystem::path& path);
Response					dynamicResponse(http::Request& request, ServerBlock& sblock, ft::filesystem::path& path);
Response					autoIndexResponse(const ft::filesystem::path& path);
std::vector<unsigned char> 	getFileContent(const ft::filesystem::path& path);

}; /* namespace http */

#endif /* SUPERVISOR_HPP */