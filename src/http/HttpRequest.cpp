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
#include "ServerConfig.hpp"
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

////////////////First line//////////////////////////
///////////////////////////////////////////
	if (_isRequestLineParsed == false)
	{
		if (_method.empty())
		{
			if (!_getCompleteToken(t, true)) return;
			if (!t.value.compare("GET") ||	!t.value.compare("POST")
			||	!t.value.compare("DELETE"))
				this->setMethod(t.value);
			else
				_code.setValue(HttpStatus::BadRequest);
				// throw std::invalid_argument("Bad http request, No method specified");
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
		{
			ph::Token cr;
			if (!_getCompleteToken(cr)) return;
			if (cr.kind != ph::ScopedEnum::kCarriage)
				_code.setValue(HttpStatus::BadRequest);
				// throw std::invalid_argument("Method line not separated by return carriage");

			if (!_getCompleteToken(t)) {
				_scanner.putback(cr);
				return;
			}
			if (t.kind != ph::ScopedEnum::kNewLine)
				_code.setValue(HttpStatus::BadRequest);
				// throw std::invalid_argument("Method line not separated by new line");
		}
		_isRequestLineParsed = true;
	}

//////////////////// Headers //////////////////////
///////////////////////////////////////////

	std::string name;
	std::string value;
	bool isValueField = false;

		// while (isHeader && (t = _scanner.getToken()).kind != ph::ScopedEnum::kEndOfInput)
	// while (!_isHeaderParsed && _getCompleteToken(t) == true)
	while (!_isHeaderParsed)
	{
		if(!_getCompleteToken(t))
		{
			if (!name.empty())
			{
				if (isValueField)
					_scanner.putback(name+ ": " + value);
				else
					_scanner.putback(name);
			}
			return ;
		}
		switch (t.kind)
		{
			case ph::ScopedEnum::kCarriage:
			{
				ph::Token nl;
				nl = _scanner.getToken();
				if (nl.kind != ph::ScopedEnum::kNewLine)
					throw std::invalid_argument("new line !>" + nl.value + "|" + ph::TokenKindToCstring(t.kind));

				if (!name.empty())
					this->addHeader(name, value);
				else
					_isHeaderParsed = true;
				name.clear();
				value.clear();
				isValueField = false;
				break;
			}
			case ph::ScopedEnum::kNewLine :
				throw std::invalid_argument("MISSING carriage before new line !!!");
				break;
			case ph::ScopedEnum::kColon :
				if (isValueField)
					value += t.value;
				else
					isValueField = true;
				break;
			case ph::ScopedEnum::kLWS :
				if (isValueField && !value.empty())
					value += t.value;
				break;
			case ph::ScopedEnum::kString :
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
	this->getUri().setAuthority(this->getHeader("Host"));

	char c;
	size_t contentLength = this->getContentLength(); // - _content.size();
	while (contentLength-- && (c = _scanner.getChar()))
		_content += c;
	if (_content.size() != this->getContentLength()) return ;
	_isComplete = true;


	if (this->getContentLength() > ServerConfig::getInstance().findServer(_uri).getClientMaxBodySize())
		_code.setValue(HttpStatus::PayloadTooLarge);
	if (this->getUri().toString().size() > 8000)
		_code.setValue(HttpStatus::URITooLong);
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
