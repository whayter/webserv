#include "parserMessage.hpp"
#include "ScannerMessage.hpp"
#include "Status.hpp"

namespace ph = parser::http;

namespace http2
{
	bool    hasTwoConsecutiverCRNL(const std::vector<unsigned char> &buffer)
	{
		std::vector<unsigned char>::const_iterator it = buffer.begin();
		std::vector<unsigned char>::const_iterator end = buffer.end();

		while (it != end)
		{
			if (*it == '\r'
			&& it + 1 != end && *(it + 1) == '\n'
			&& it + 2 != end && *(it + 2) == '\r'
			&& it + 3 != end && *(it + 3) == '\n')
				return true;
			++it;
		}
		return false;
	}

	/// return true if a request has been parsed, else, false.
	bool parseRequest(http::Request &request, http::Status &error, std::vector<unsigned char> &buffer)
	{
		ph::Token t;

		error = http::Status::None;
		if (hasTwoConsecutiverCRNL(buffer))
			return false;

		(void)request;
		(void)error;

		// if (!_getCompleteToken(t, true)) return;
		// if (!t.value.compare("GET") ||	!t.value.compare("POST")
		// ||	!t.value.compare("DELETE"))
		// 	this->setMethod(t.value);
		// else
		// 	error = http::Status::BadRequest; // throw std::invalid_argument("Bad http request, No method specified");

		// if (!_getCompleteToken(t, true)) return;
		// this->setUri(Uri("http", t.value));

		// if (!_getCompleteToken(t, true)) return;
		// this->setVersion(t.value);
		// ph::Token cr;

		// if (!_getCompleteToken(cr)) return;
		// if (cr.kind != ph::TokenKind::kCarriage)
		// 	error = http::Status::BadRequest;  // throw std::invalid_argument("Method line not separated by return carriage");

		// if (!_getCompleteToken(t)) {
		// 	_scanner.putback(cr);
		// 	return;
		// }
		// if (t.kind != ph::TokenKind::kNewLine)
		// 	error = setValue(http::Status::BadRequest); // throw std::invalid_argument("Method line not separated by new line");
		return true;
	}
}
