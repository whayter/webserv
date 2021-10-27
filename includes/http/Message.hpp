#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "http.hpp"

namespace http {

class Message {
	
public:

	Message();
	virtual ~Message() {}

	inline headers_type&	getHeaders() { return _headers; }
	inline content_type&	getContent() { return _content; }
	inline std::string		getVersion() { return _version; }

	std::string 			getHeader(std::string name);
	size_t 					getContentLength();

	void 					setHeader(std::string key, std::string val);
	void					setContent(const content_type &content);
	void					setContent(const content_type &content, std::string contentType);
	void					setVersion(const std::string &version);

	void					delHeader(std::string name);

	virtual bool			empty() const;
	virtual void			clear();

protected:

	headers_type			_headers;
	content_type			_content;
	std::string				_version;

}; /* class Message */

} /* namespace http */

#endif /* MESSAGE_HPP */