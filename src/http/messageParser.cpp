#include "messageParser.hpp"
#include "ScannerMessage.hpp"
#include "Status.hpp"
#include "Uri.hpp"
#include "Request.hpp"
#include "Context.hpp"
#include "utility.hpp"

namespace ph = parser::http;

#define IAC 0xff

namespace http
{
	static void removeTrailingCRNL(std::vector<unsigned char> &buffer)
	{
		size_t i = 0;

		while (i + 1 < buffer.size() && buffer[i] == '\r' && buffer[i+1] == '\n')
			i += 2;
		buffer.erase(buffer.begin(), buffer.begin() + i);
	}

	bool hasTwoConsecutiveCRNL(const std::vector<unsigned char> &buffer, bool &endOfInput)
	{
		std::vector<unsigned char>::const_iterator it = buffer.begin();
		std::vector<unsigned char>::const_iterator end = buffer.end();

		endOfInput = false;
		while (it != end)
		{
			if (*it == IAC)
			{
				endOfInput = true;
				return false;
			}
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

	// return false on error
	static void parseRequestLine(ph::ScannerMessage &scan, http::Request &req, http::Status &error)
	{
		ph::Token t = scan.getToken(true);
		{
			if (t.kind == ph::TokenKind::kEndOfInput)
			{
				setError(error, http::Status::EndOfInput);
				return;
			}
			else if (!t.value.compare("GET") || !t.value.compare("POST") || !t.value.compare("DELETE"))
				req.setMethod(t.value);
			else if (!t.value.compare("PUT") || !t.value.compare("PATCH") || !t.value.compare("COPY"))
				setError(error, http::Status::NotImplemented);
			else
				setError(error, http::Status::BadRequest);
		}
		{
			t = scan.getToken(true);
			if (t.kind == ph::TokenKind::kEndOfInput)
				setError(error, http::Status::EndOfInput);
			else if (t.kind != ph::TokenKind::kString)
				setError(error, http::Status::BadRequest);
			else
				req.setUri(Uri("http", t.value));
		}
		{
			t = scan.getToken(true);
			if (t.kind == ph::TokenKind::kEndOfInput)
				setError(error, http::Status::EndOfInput);
			if (t.value != "HTTP/1.1")
			{
				if (!t.value.compare(0, 5, "HTTP/") && t.value.size() > 5)
					setError(error, http::Status::HTTPVersionNotSupported);
				setError(error, http::Status::BadRequest);
			}
			req.setVersion(t.value);
		}
		{
			t = scan.getToken(true);
			if (t.kind == ph::TokenKind::kEndOfInput)
				setError(error, http::Status::EndOfInput);
			if (t.kind != ph::TokenKind::kCarriage)
				setError(error, http::Status::BadRequest);
		}
		{
			t = scan.getToken(true);
			if (t.kind == ph::TokenKind::kEndOfInput)
				setError(error, http::Status::EndOfInput);
			if (t.kind != ph::TokenKind::kNewLine)
				setError(error, http::Status::BadRequest);
		}
	}

	void parseHeaders(ph::ScannerMessage &scan, http::Message &req, http::Status &error)
	{
		ph::Token t;

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
					{
						setError(error, http::Status::BadRequest);
						return;
					}
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
					setError(error,  http::Status::BadRequest);
					return;
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
				
				case ph::TokenKind::kEndOfInput :
					setError(error,  http::Status::BadRequest);
					return;
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
		removeTrailingCRNL(buffer);
		bool endOfInput = false;
		if (!hasTwoConsecutiveCRNL(buffer, endOfInput))
		{
			if (endOfInput)
			{
				setError(error, http::Status::EndOfInput);
				return true;
			}
			return false;
		}
		ph::ScannerMessage scan(buffer);
		parseRequestLine(scan, req, error);
		if (error == http::Status::BadRequest || error == http::Status::EndOfInput)
			return true;
		parseHeaders(scan, req, error);
		if (req.getHeader("Host").empty())
			setError(error, http::Status::BadRequest);
		if (error == http::Status::BadRequest || error == http::Status::EndOfInput)
			return true;
		req.getUri().setAuthority(req.getHeader("Host"));
		{			
			char c;
			size_t contentLength = req.getContentLength();
			if (contentLength > getContext(req.getUri()).location.getClientMaxBodySize())
			{
				setError(error, http::Status::PayloadTooLarge);
				return true;
			}
			if (scan.remainCharCount() < contentLength)
				return false;
			while (contentLength-- && (c = scan.getChar()))
				req.getContent().push_back(c);
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
