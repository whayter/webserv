/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerHttpRequest.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 13:39:02 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/21 15:26:15 by juligonz         ###   ########.fr       */
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

Token ScannerHttpRequest::getToken(bool skipLWS)
{
	char c = 0;

	if (skipLWS)
		do
		{
			c = _scan.get();
		} while (c != '\n' && isspace(c));
	else
		c = _scan.get();
	
	switch (c)
	{
		case  -1:
		case  0 :	return _makeToken(ScopedEnum::kEndOfInput, "");
		case ':':	return _makeToken(ScopedEnum::kColon, ":");
		case ',':	return _makeToken(ScopedEnum::kComma, ",");
		case '\n':	return _makeToken(ScopedEnum::kNewLine, "\n");
		case '\r':	return _makeToken(ScopedEnum::kCarriage, "\r");
		case ' ':	return _makeToken(ScopedEnum::kLWS, " ");
		case '\t':	return _makeToken(ScopedEnum::kLWS, "\t");
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
					_scan.putback(c);
				return _makeToken(ScopedEnum::kString, lexeme);
			}
			return _makeToken(ScopedEnum::kError,
						std::string("Cant parse lexeme:\"" + lexeme +"\", char:'" + c+ "'  "));
	}
}

char ScannerHttpRequest::getChar()
{
	return _scan.get();
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
	t.value = value;
	t.column = _scan.getColumn();
	t.line = _scan.getLine();
	return t;
}

std::ostream & operator <<(std::ostream& os, const Token &t)
{
	os << "<" << TokenKindToCstring(t.kind);
	switch (t.kind)
	{
		case (ScopedEnum::kString)		:	os << "=\"" << t.value << "\"> ";	break;
		case (ScopedEnum::kError)		:	os << "=\"" << t.value << "\"> ";	break;
		default							:	os << "> "; break;
	}
	return os;
}

const char* TokenKindToCstring(TokenKind type)
{
	static const char* str[] = {
		"kEnfOfInput", "kError",
		"kString", "kNewLine", "kCarriage",
		"kLeftBrace", "kRightBrace",
		"kComma", "kColon", "kLWS"
	};
	return str[type];
}

} /* namespace http */
} /* namespace parser */
