#ifndef PARSER_MESSAGE_HPP
#define PARSER_MESSAGE_HPP

#include "AMessage.hpp"

namespace http2 {

bool    hasTwoConsecutiverCRNL(const std::vector<unsigned char> &buffer);

bool    parseRequest(http::Request &request, http::Status &error, std::vector<unsigned char> &buffer);


} /* namespace http2 */

#endif /* PARSER_MESSAGE_HPP */
