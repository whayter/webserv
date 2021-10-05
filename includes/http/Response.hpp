#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "http.hpp"
#include "Message.hpp"
#include "Status.hpp"

namespace http {

class Response: public Message {

public:

	Response();
	~Response();

	inline Status&	getStatus() { return _status; }

	void			setStatus(Status e);
	bool			empty() const;
	void			clear();

private:

	Status			_status;

}; /* class Response */

}; /* namespace http */

#endif /* REPSONSE_HPP */