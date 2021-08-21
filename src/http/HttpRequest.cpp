/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 14:36:30 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/21 17:47:19 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/HttpRequest.hpp"
#include "parser/http/ScannerHttpRequest.hpp"

HttpRequest::HttpRequest()
	: AHttpMessage(), _isHeaderParsed(false), _isContentParsed(false) {}

HttpRequest::~HttpRequest() {}

// HttpRequest::HttpRequest(HttpRequest &other):
// 	AHttpMessage()
// {}

// HttpRequest& operator=(const HttpRequest &)
// {
	
// }


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
	// s += "Parameters:\n\t" + _queryString + "\n";
	s += "Headers:\n";
	header = this->getHeaders();
	for (it = header.begin(); it != header.end(); it++)
		s += "\t" + it->first + "\t" + it->second + "\n";
	return s;
}


namespace ph = parser::http;
void HttpRequest::read(std::istream & inputStream)
{
	ph::ScannerHttpRequest scanner(inputStream);

	ph::Token t = scanner.getToken(true);
	if (!t.value.compare("GET") ||	!t.value.compare("POST")
		||	!t.value.compare("DELETE"))
	{
		this->setMethod(t.value);
		
		t = scanner.getToken(true);
		this->setUri(Uri("http", t.value));
		
		t = scanner.getToken(true);
		this->setVersion(t.value);
	}
	else
		throw std::invalid_argument("Bad http request, No method specified");

	t = scanner.getToken();
	if (ph::ScopedEnum::kCarriage != t.kind)
		throw std::invalid_argument("Method line not separated by return carriage");
	t = scanner.getToken();
	if (ph::ScopedEnum::kNewLine != t.kind)
		throw std::invalid_argument("Method line not separated by new line");

	std::string name;
	std::string value;
	bool isValueField = false;
	bool isHeader = true;
	bool lastIsCariage = false;
	while (isHeader && (t = scanner.getToken()).kind != ph::ScopedEnum::kEndOfInput)
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
	this->getUri().setAuthority(this->getHeader("Host"));

	std::string content;
	char c;
	size_t contentLength = this->getContentLength();
	while (contentLength-- && (c = scanner.getChar()) != -1)
		content += c;
	this->setContent(content);
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
	_isHeaderParsed = false;
	_isContentParsed = false;
}


HttpRequest HttpRequest::create(std::istream & inputStream)
{
	ph::ScannerHttpRequest scanner(inputStream);
	HttpRequest result;

	ph::Token t = scanner.getToken(true);
	if (!t.value.compare("GET") ||	!t.value.compare("POST")
		||	!t.value.compare("DELETE"))
	{
		result.setMethod(t.value);
		
		t = scanner.getToken(true);
		result.setUri(Uri("http", t.value));
		
		t = scanner.getToken(true);
		result.setVersion(t.value);
	}
	else
		throw std::invalid_argument("Bad http request, No method specified");

	t = scanner.getToken();
	if (ph::ScopedEnum::kCarriage != t.kind)
		throw std::invalid_argument("Method line not separated by return carriage");
	t = scanner.getToken();
	if (ph::ScopedEnum::kNewLine != t.kind)
		throw std::invalid_argument("Method line not separated by new line");

	std::string name;
	std::string value;
	bool isValueField = false;
	bool isHeader = true;
	bool lastIsCariage = false;
	while (isHeader && (t = scanner.getToken()).kind != ph::ScopedEnum::kEndOfInput)
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
					result.addHeader(name, value);
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
	result.getUri().setAuthority(result.getHeader("Host"));

	std::string content;
	char c;
	size_t contentLength = result.getContentLength();
	while (contentLength-- && (c = scanner.getChar()) != -1)
		content += c;
	result.setContent(content);
	return result;
}