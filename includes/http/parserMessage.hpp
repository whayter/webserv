#ifndef PARSER_MESSAGE_HPP
#define PARSER_MESSAGE_HPP

#include <vector>

namespace parser{ namespace http {
	class ScannerMessage;
}} /* namespace parser::http */

namespace http{
	class Request;
	class Message;
	struct Status;

	bool	hasTwoConsecutiveCRNL(const std::vector<unsigned char> &buffer);

	void	parseHeaders(parser::http::ScannerMessage &scan, http::Message &req, http::Status &error);
	bool	parseRequest(http::Request &request, http::Status &error, std::vector<unsigned char> &buffer);

	http::Message parseCgiResponse(std::vector<unsigned char> &buffer);
} /* namespace http */

#endif /* PARSER_MESSAGE_HPP */
