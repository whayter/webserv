/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerHttpRequest.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 13:39:02 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/06 15:06:37 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/http/ScannerHttpRequest.hpp"

namespace parser
{
namespace http
{
	
ScannerHttpRequest::ScannerHttpRequest(std::istream &inputStream)
	: _scan(inputStream)
{}

ScannerHttpRequest::~ScannerHttpRequest(){}

Token ScannerHttpRequest::getToken()
{
	char c = 0;

	do
	{
		c = _scan.get();
	} while (c != '\n' && isspace(c));

	switch (c)
	{
		case  -1:
		case  0 :	return _makeToken(ScopedEnum::kEndOfInput, "");
		case ':':	return _makeToken(ScopedEnum::kColon, ":");
		case ',':	return _makeToken(ScopedEnum::kComma, ",");
		case '\n':	return _makeToken(ScopedEnum::kNewLine, ",");
		default:
			std::string lexeme = "";
			if (_charIsString(c))
			{
				while (_charIsString(c))
				{
					lexeme += c;
					c = _scan.get();
				}
				if (!_charIsString(c))
					_scan.unget();
				return _makeToken(ScopedEnum::kString, lexeme);
			}
			return _makeToken(ScopedEnum::kError, "Cant parse lexeme");
	}
}

/// Must only be called in the switch statement
bool ScannerHttpRequest::_charIsString(char c){
	if (c == ':' || isspace(c))
		return false;
	if (isprint(c))
		return true;
	return false;
}

Token ScannerHttpRequest::_makeToken(TokenKind kind, std::string value)
{
	Token t;

	t.kind = kind;
	t.valueString = value;
	t.column = _scan.getColumn();
	t.line = _scan.getLine();
	return t;
}

std::ostream & operator <<(std::ostream& os, const Token &t)
{
	os << "<" << TokenKindToCstring(t.kind);
	switch (t.kind)
	{
		case (ScopedEnum::kString)		:	os << "=\"" << t.valueString << "\"> ";	break;
		case (ScopedEnum::kError)		:	os << "=\"" << t.valueString << "\"> ";	break;
		default							:	os << "> "; break;
	}
	return os;
}

const char* TokenKindToCstring(TokenKind type)
{
	static const char* str[] = {
		"kEnfOfInput", "kError",
		"kString", "kNewLine",
		"kLeftBrace", "kRightBrace",
		"kComma", "kColon",
	};
	return str[type];
}

} /* namespace http */
} /* namespace parser */
