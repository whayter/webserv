#include "Message.hpp"
#include "utility.hpp"
#include <cstdlib>

namespace http {

Message::Message() {}

// Message::~Message() {}

std::string	Message::getHeader(std::string name)
{
	http::headers_type::const_iterator pos = getHeaders().find(name);

	if (pos != getHeaders().end())
		return pos->second;
	return std::string();
}

void	Message::delHeader(std::string name)
{
	http::headers_type::iterator pos = getHeaders().find(name);

	if (pos != getHeaders().end())
		getHeaders().erase(pos);
}

size_t Message::getContentLength()
{
	http::headers_type::const_iterator pos = getHeaders().find("Content-Length");
	if (pos != getHeaders().end())
		return ::strtoul(pos->second.c_str(), 0, 10);	
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

void Message::setContent(const content_type &content, std::string contentType)
{
	_content = content;
	setHeader("Content-Length", ft::intToString(content.size()));
	setHeader("Content-Type", contentType);
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