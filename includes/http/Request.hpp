#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "http.hpp"
#include "Message.hpp"
#include "Uri.hpp"

namespace http {

class Request: public Message {

public:

	Request();
	~Request();

	inline std::string		getMethod() { return _method; }
	inline Uri&				getUri() { return _uri; }

	void					setMethod(std::string method);
	void					setUri(const Uri& uri);
	bool					empty() const;
	void					clear();

private:

	std::string				_method;
	Uri						_uri;

}; /* class Request */

}; /* namespace http */

#endif /* REQUEST_HPP */