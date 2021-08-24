/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 14:36:30 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/21 19:24:42 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HttpRequest.hpp"
#include "parser/http/ScannerHttpRequest.hpp"

HttpRequest::HttpRequest()
	: AHttpMessage(), _code(HttpStatus::None), _isRequestLineParsed(false),
		_isHeaderParsed(false),	_isComplete(false), _scanner(NULL)
{}

HttpRequest::~HttpRequest() {}


std::string& HttpRequest::getMethod()
{
	return _method;
}

Uri&	HttpRequest::getUri()
{
	return _uri;
}


void HttpRequest::setMethod(std::string method)
{
	_method = method;
}

void HttpRequest::setUri(const Uri &uri)
{
	_uri = uri;
}

void HttpRequest::setVersion(std::string version)
{
	_version = version;
}

std::string HttpRequest::toString()
{
	std::string s;
	map_type header;
	map_type::iterator it;

	s = "Method:\n\t" + getMethod() + "\n";
	s += "URI:\n\t" + _uri.toString() + "\n";
	s += "Headers:\n";
	header = this->getHeaders();
	for (it = header.begin(); it != header.end(); it++)
		s += "\t" + it->first + "\t" + it->second + "\n";
	return s;
}


namespace ph = parser::http;

void HttpRequest::read(const char *buffer)
{
	ph::Token t;
	
	// if (isComplete())
	// 	throw std::logic_error("Wtf ima complete request already");
	_scanner.pushNewBuffer(buffer);

	// if (!_getCompleteToken(t, true)) return;
	// if (!_getCompleteToken(t, true)){std::cout << "TOk" << t << std::endl; return;}
	// t = _scanner.getToken(true);

	if (_isRequestLineParsed == false)
	{
		if (_method.empty())
		{
			if (!_getCompleteToken(t, true)) return;
			if (!t.value.compare("GET") ||	!t.value.compare("POST")
			||	!t.value.compare("DELETE"))
				this->setMethod(t.value);
			else
				throw std::invalid_argument("Bad http request, No method specified");
			// _code.setValue(HttpStatus::BadRequest);
		}
		if (_uri.empty())
		{		
			if (!_getCompleteToken(t, true)) return;
			this->setUri(Uri("http", t.value));
				
		}
		if (_version.empty())
		{		
			if (!_getCompleteToken(t, true)) return;
			this->setVersion(t.value);
		}
		if (!_getCompleteToken(t)) return;
		if (ph::ScopedEnum::kCarriage != t.kind)
			throw std::invalid_argument("Method line not separated by return carriage");
			// _code.setValue(HttpStatus::BadRequest);

		if (!_getCompleteToken(t)) return;
		if (ph::ScopedEnum::kNewLine != t.kind)
			throw std::invalid_argument("Method line not separated by new line");
			// _code.setValue(HttpStatus::BadRequest);
		
		_isRequestLineParsed = true;
	}

	std::string name;
	std::string value;
	bool isValueField = false;
	bool isHeader = true;
	bool lastIsCariage = false;
	while (isHeader && (t = _scanner.getToken()).kind != ph::ScopedEnum::kEndOfInput)
	{
		switch (t.kind)
		{
			case ph::ScopedEnum::kCarriage:
				lastIsCariage = true;
				break;
			case ph::ScopedEnum::kNewLine :
				if (lastIsCariage == false)
					throw std::invalid_argument("MISSING carriage before new line !!!");
				if (!name.empty())
					this->addHeader(name, value);
				else
					isHeader = false;
				name.clear();
				value.clear();
				lastIsCariage = false;
				isValueField = false;
				break;
			case ph::ScopedEnum::kColon :
				lastIsCariage = false;
				if (isValueField)
					value += t.value;
				else
					isValueField = true;
				break;
			case ph::ScopedEnum::kLWS :
				lastIsCariage = false;
				if (isValueField && !value.empty())
					value += t.value;
				break;
			case ph::ScopedEnum::kString :
				lastIsCariage = false;
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
	_isHeaderParsed = true;
	this->getUri().setAuthority(this->getHeader("Host"));

	char c;
	size_t contentLength = this->getContentLength();
	while (contentLength-- && (c = _scanner.getChar()) != -1)
		_content += c;
	_isComplete = true;
}

bool HttpRequest::_getCompleteToken(ph::Token& placeHolder, bool skipLWS)
{
	placeHolder = _scanner.getToken(skipLWS);
	if (_scanner.peekNextToken().kind == ph::ScopedEnum::kEndOfInput)
	{
		_scanner.putback(placeHolder);
		return false;
	}
	return true;
}

void HttpRequest::write(std::ostream os)
{
	map_type header;
	map_type::iterator it;

	os << _method << " ";
	os << _uri.getPathEtc() << " ";
	os << _version << "\n";
	for (it = header.begin(); it != header.end(); it++)
		os << it->first << ": " << it->second + "\n";
	os.flush();
}

void	HttpRequest::clear(void)
{
	_method.clear();
	_uri.clear();
	_version.clear();
	_content.clear();
	_headers.clear();
	_isRequestLineParsed = false;
	_isHeaderParsed = false;
	_isComplete = false;
}

bool	HttpRequest::isComplete(void)
{
	return _isComplete;
}

int	HttpRequest::getHttpErrorCode()
{
	return _code.getValue();
}
