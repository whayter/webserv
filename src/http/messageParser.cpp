#include "messageParser.hpp"
#include "ScannerMessage.hpp"
#include "Status.hpp"
#include "Uri.hpp"
#include "Request.hpp"
#include "Context.hpp"
#include "utility.hpp"

#include <climits>
#include <cctype>

namespace ph = parser::http;

#define IAC 0xff

#define MAX_HTTP_HEADER_SIZE 5000 // bytes == 5 MO/MB

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
		if (error != http::Status::None && value != http::Status::BadRequest)
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
			else if (t.kind != ph::TokenKind::kString || t.value[0] != '/')
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
						req.setHeader(name, ft::trim(value));
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

	static bool parseChunkedContent(ph::ScannerMessage &scan, http::Request &req, http::Status &error)
	{
		Context context = getContext(req.getUri());

		size_t totalSize = 0;
		while (true)
		{
			ph::Token t = scan.getToken(false);
			if (t.kind == ph::TokenKind::kEndOfInput)
				return false;

			char *nptr;
			unsigned long chunkLength = strtoul(t.value.c_str(), &nptr, 16);
			if ((chunkLength == ULONG_MAX && ft::make_error_code().value() == ft::errc::result_out_of_range)
				|| *nptr)
			{
				setError(error, http::Status::BadRequest);
				return true;
			}
			if ((t = scan.getToken(false)).kind != ph::TokenKind::kCarriage
				|| (t = scan.getToken(false)).kind != ph::TokenKind::kNewLine)
			{
				if (t.kind == ph::TokenKind::kEndOfInput)
					return false;
				setError(error, http::Status::BadRequest);
				return true;
			}
			if (chunkLength == 0)
				break;
			totalSize += chunkLength;
			if (totalSize > context.location.getClientMaxBodySize())
			{
				setError(error, http::Status::PayloadTooLarge);
				return true;
			}
			if (scan.remainCharCount() < chunkLength)
				return false;
			while (chunkLength--)
				req.getContent().push_back(scan.getChar());
			if ((t = scan.getToken(false)).kind != ph::TokenKind::kCarriage
				|| (t = scan.getToken(false)).kind != ph::TokenKind::kNewLine)
			{
				if (t.kind == ph::TokenKind::kEndOfInput)
					return false;
				setError(error, http::Status::BadRequest);
				return true;
			}
		}
		req.setHeader("Content-Length", ft::intToString(totalSize));
		req.delHeader("Transfer-Encoding");
		scan.eraseBeforeCurrentIndex();
		return true;
	}

	static bool parseContent(ph::ScannerMessage &scan, http::Request &req, http::Status &error)
	{
		Context context = getContext(req.getUri());
		size_t contentLength = req.getContentLength();

		if (contentLength > context.location.getClientMaxBodySize())
		{
			setError(error, http::Status::PayloadTooLarge);
			return true;
		}
		if (scan.remainCharCount() < contentLength)
			return false;
		while (contentLength--)
			req.getContent().push_back(scan.getChar());
		scan.eraseBeforeCurrentIndex();
		return true;
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
			if (buffer.size() && !::isupper(buffer[0]))
				setError(error, http::Status::BadRequest);
			else if (endOfInput)
				setError(error, http::Status::EndOfInput);
			else if (buffer.size() > MAX_HTTP_HEADER_SIZE)
				setError(error, http::Status::RequestHeaderFieldsTooLarge);
			else
				return false;
			return true;
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

		if (req.getHeader("Transfer-Encoding") == "chunked")
			return parseChunkedContent(scan, req, error);
		if(req.getHeader("Transfer-Encoding").empty())
			return parseContent(scan, req, error);
		// else transfer-encoding not supported
		setError(error, http::Status::NotImplemented);
		return true;
	}

	http::Message parseCgiResponse(std::vector<unsigned char> &buffer)
	{
		ph::ScannerMessage scan(buffer);
		http::Message resp;

		http::Status error; // useless but needed
		parseHeaders(scan, resp, error);
		{
			char c;
			while ((c = scan.getChar()))
				resp.getContent().push_back(c);
		}
		return resp;
	}

} /*namespace http */
