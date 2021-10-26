#ifndef MESSAGE_BUILDER_HPP
#define MESSAGE_BUILDER_HPP

#include "http.hpp"
#include "utility.hpp"
#include "ServerConfig.hpp"
#include "Context.hpp"
#include "pageBuilding.hpp"

namespace http {

Response					buildResponse(Request& request);
Response					staticResponse(const Context& ctxt, Response& response);
Response					dynamicResponse(const Context& ctxt, Request& request, Response& response);
Response					postResponse(const Context& ctxt, Request& request, Response& response);
Response					deleteResponse(const Context& ctxt, Response& response);
Response					redirectResponse(const Context& ctxt, Response& response);
Response					autoIndexResponse(const Context& ctxt, Request& request, Response& response);
Response					errorResponse(const Context& ctxt, Response& response, Status error);
int							postContent(std::string path, content_type content);
std::vector<unsigned char> 	getFileContent(const ft::filesystem::path& path);

template <class Message>
std::string					stringifyMessage(Message& message)
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

}; /* namespace http */

#endif /* SUPERVISOR_HPP */