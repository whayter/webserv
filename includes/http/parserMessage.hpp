#ifndef PARSER_MESSAGE_HPP
#define PARSER_MESSAGE_HPP

#include <vector>

namespace parser{ namespace http {
	class ScannerMessage2;
}} /* namespace parser::http */

namespace http{
	class Request;
	struct Status;

	bool	hasTwoConsecutiverCRNL(const std::vector<unsigned char> &buffer);

	void	parseHeaders(parser::http::ScannerMessage2 &scan, http::Request &req, http::Status &error);
	bool	parseRequest(http::Request &request, http::Status &error, std::vector<unsigned char> &buffer);

} /* namespace http */

#endif /* PARSER_MESSAGE_HPP */
