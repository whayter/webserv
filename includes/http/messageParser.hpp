#ifndef MESSAGE_PARSER_HPP
#define MESSAGE_PARSER_HPP

#include "http.hpp"
#include "ScannerMessage.hpp"

#include <vector>

namespace http {

	bool	hasTwoConsecutiveCRNL(const content_type &buffer, bool &endOfInput);
	void	parseHeaders(parser::http::ScannerMessage &scan, Message &req, Status &error);
	bool	parseRequest(Request &request, Status &error, content_type &buffer);
	Message parseCgiResponse(content_type &buffer);

	struct multipart_part
	{
		std::map<std::string, std::string> headers;
		unsigned char 	*content;
		size_t 	len;

		std::string getFilename() const;
	};

	http::Status parseContentMultipart(std::vector<multipart_part>& result,
										http::Request& request, const std::string& boundary);


} /* namespace http */

#endif /* MESSAGE_PARSER_HPP */
