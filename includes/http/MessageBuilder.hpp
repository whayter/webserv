#ifndef MESSAGE_BUILDER_HPP
#define MESSAGE_BUILDER_HPP

#include "http.hpp"
#include "utility.hpp"
#include "ServerConfig.hpp"
#include "pageBuilding.hpp"

namespace http {

Response					buildResponse(Request& request);
Response					errorResponse(const Uri& uri, Status error);
Response					redirectResponse(const ReturnDirective &rdir);
Response					staticResponse(const ft::filesystem::path& path);
Response					dynamicResponse(http::Request& request, ServerBlock& sblock, ft::filesystem::path& path);
Response					autoIndexResponse(const ft::filesystem::path& path);
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