#include "Message.hpp"
#include "utility.hpp"
#include <cstdlib>

namespace http {

Message::Message() {}

// Message::~Message() {}

std::string	Message::getHeader(std::string name)
{
	return _headers[name];
}

size_t Message::getContentLength()
{
	const char* key = "Content-Length";
	http::headers_type headers = getHeaders();
	if (headers.find(key) != headers.end())
		return ::strtoul(headers[key].c_str(), 0, 10);	
	return 0;
}

void Message::setHeader(std::string key, std::string val)
{
	_headers[key] = val;
}

void Message::setContent(const content_type &content)
{
	_content = content;
	setHeader("Content-Length", ft::intToString(content.size()));
}

void Message::setVersion(const std::string &version)
{
	_version = version;
}

bool Message::empty() const
{
	if (_headers.empty() && _content.empty() && _version.empty())
		return true;
	return false;	
}

void Message::clear()
{
	_headers.clear();
	_content.clear();
	_version.clear();
};

} /* namespace http */