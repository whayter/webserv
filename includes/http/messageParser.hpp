#ifndef MESSAGE_PARSER_HPP
#define MESSAGE_PARSER_HPP

#include "http.hpp"
#include "ScannerMessage.hpp"

#include <vector>

namespace http {

	bool	hasTwoConsecutiveCRNL(const content_type &buffer);
	void	parseHeaders(parser::http::ScannerMessage &scan, Message &req, Status &error);
	bool	parseRequest(Request &request, Status &error, content_type &buffer);
	Message parseCgiResponse(content_type &buffer);

} /* namespace http */

#endif /* MESSAGE_PARSER_HPP */
