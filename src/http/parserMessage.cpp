#include "parserMessage.hpp"
#include "ScannerMessage2.hpp"
#include "Status.hpp"
#include "Uri.hpp"
#include "Request.hpp"

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

	namespace ph = parser::http;

	static void setError(http::Status &error, http::Status value)
	{
		if (error != http::Status::None)
			return;
		error = value;
	}

	/// return true if a request has been parsed, else, false.
	bool parseRequest(http::Request &req, http::Status &error, std::vector<unsigned char> &buffer)
	{
		ph::Token t;

		error = http::Status::None;
		if (!hasTwoConsecutiverCRNL(buffer))
			return false;
		ph::ScannerMessage2 scan(buffer);

////////////////First line//////////////////////////
///////////////////////////////////////////
		t = scan.getToken(true);
		if (!t.value.compare("GET") ||	!t.value.compare("POST") ||	!t.value.compare("DELETE"))
			req.setMethod(t.value);
		else
			setError(error, http::Status::BadRequest); // throw std::invalid_argument("Bad http request, No method specified");

		t = scan.getToken(true);
		req.setUri(Uri("http", t.value));

		t = scan.getToken(true);
		req.setVersion(t.value);
		
		t = scan.getToken(true);
		if (t.kind != ph::TokenKind::kCarriage)
			setError(error, http::Status::BadRequest); // throw std::invalid_argument("Method line not separated by return carriage");
		t = scan.getToken(true);
		if (t.kind != ph::TokenKind::kNewLine)
			setError(error, http::Status::BadRequest); // throw std::invalid_argument("Method line not separated by new line");

//////////////////// Headers //////////////////////
///////////////////////////////////////////
		std::string name;
		std::string value;
		bool isValueField = false;
		bool areHeadersParsed = false;
		while (!areHeadersParsed)
		{
			t = scan.getToken();
			switch (t.kind.getValue())
			{
				case ph::TokenKind::kCarriage:
				{
					t = scan.getToken();
					if (t.kind != ph::TokenKind::kNewLine)
						throw std::invalid_argument("new line !>" + t.value + "|" + ph::TokenKindToCstring(t.kind));

					if (!name.empty())
						req.setHeader(name, value);
					else
						areHeadersParsed = true;
					name.clear();
					value.clear();
					isValueField = false;
					break;
				}
				case ph::TokenKind::kNewLine :
					throw std::invalid_argument("MISSING carriage before new line !!!");
					break;
				case ph::TokenKind::kColon :
					if (isValueField)
						value += t.value;
					else
						isValueField = true;
					break;
				case ph::TokenKind::kLWS :
					if (isValueField && !value.empty())
						value += t.value;
					break;
				case ph::TokenKind::kString :
					if (isValueField == false)
						name += t.value;
					else
						value += t.value;
					break;
				
				default:
					throw "Ho shit";
					break;
			}
		}
		req.getUri().setAuthority(req.getHeader("Host"));

		char c;
		size_t contentLength = req.getContentLength();
		while (contentLength-- && (c = scan.getChar()))
			req.getContent().push_back(c);
		if (req.getContent().size() != req.getContentLength())
			return false;
		return true;
	}

} /*namespace http2 */
