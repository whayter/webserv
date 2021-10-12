#include "messageParser.hpp"
#include "ScannerMessage.hpp"
#include "Status.hpp"
#include "Uri.hpp"
#include "Request.hpp"
#include "Context.hpp"

namespace ph = parser::http;

namespace http
{
	bool hasTwoConsecutiveCRNL(const std::vector<unsigned char> &buffer)
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

	static void setError(http::Status &error, http::Status value)
	{
		if (error != http::Status::None)
			return;
		error = value;
	}

	static void parseRequestLine(ph::ScannerMessage &scan, http::Request &req, http::Status &error)
	{
		ph::Token t = scan.getToken(true);
		if (!t.value.compare("GET") || !t.value.compare("POST") || !t.value.compare("DELETE"))
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
	}

	void parseHeaders(ph::ScannerMessage &scan, http::Message &req, http::Status &error)
	{
		ph::Token t;

		(void)error;
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
						throw std::invalid_argument("new line !>" + t.value + "|" + ph::tokenKindToCstring(t.kind));

					if (!name.empty() && isValueField)
						req.setHeader(name, value);
					else if (name.empty())
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
					throw std::runtime_error("Oups: unexpected token " + ph::tokenToString(t));
					break;
			}
		}
	}

	// return true if a request has been parsed, else, false.
	bool parseRequest(http::Request &req, http::Status &error, std::vector<unsigned char> &buffer)
	{
		req.clear();
		error = http::Status::None;
		if (!hasTwoConsecutiveCRNL(buffer))
			return false;
		ph::ScannerMessage scan(buffer);
		parseRequestLine(scan, req, error);
		parseHeaders(scan, req, error);
		req.getUri().setAuthority(req.getHeader("Host"));
		{			
			char c;
			bool recordContent = true;
			size_t count = 0;
			size_t contentLength = req.getContentLength();
			if (contentLength > getContext(req.getUri()).location.getClientMaxBodySize())
			{
				setError(error, 413);
				recordContent = false;
			}
			while (contentLength-- && (c = scan.getChar()))
			{
				if (recordContent)
					req.getContent().push_back(c);
				count++;
			}
			if (count != req.getContentLength())
				return false;
		}
		scan.eraseBeforeCurrentIndex();
		return true;
	}

	http::Message parseCgiResponse(std::vector<unsigned char> &buffer)
	{
		ph::ScannerMessage scan(buffer);
		http::Message resp;

		http::Status error; // useless
		parseHeaders(scan, resp, error);
		{
			char c;
			while ((c = scan.getChar()))
				resp.getContent().push_back(c);
		}
		return resp;
	}

} /*namespace http */
