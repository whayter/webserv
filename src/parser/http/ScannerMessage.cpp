/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerMessage.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 13:39:02 by juligonz          #+#    #+#             */
/*   Updated: 2021/09/05 13:16:58 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/http/ScannerMessage.hpp"

namespace parser
{
namespace http
{

ScannerMessage::ScannerMessage(const char *buffer)
	: _scan(buffer)
{}

ScannerMessage::~ScannerMessage() {}

Token ScannerMessage::getToken(bool skipLWS)
{
	char c = 0;

	if (skipLWS)
		do
		{
			c = _scan.get();
		} while (c != '\n' && c != '\r' && isspace(c));
	else
		c = _scan.get();
	
	switch (c)
	{
		case  -1:
		case  0 :	return _makeToken(TokenKind::kEndOfInput, "");
		case ':':	return _makeToken(TokenKind::kColon, ":");
		case ',':	return _makeToken(TokenKind::kComma, ",");
		case '\n':	return _makeToken(TokenKind::kNewLine, "\n");
		case '\r':	return _makeToken(TokenKind::kCarriage, "\r");
		case ' ':	return _makeToken(TokenKind::kLWS, " ");
		case '\t':	return _makeToken(TokenKind::kLWS, "\t");
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
				return _makeToken(TokenKind::kString, lexeme);
			}
			return _makeToken(TokenKind::kError,
						std::string("Cant parse lexeme:\"" + lexeme +"\", char:'" + c+ "'  "));
	}
}

char ScannerMessage::getChar()
{
	return _scan.get();
}
Token ScannerMessage::peekNextToken(bool skipLWS)
{
	Token result = getToken(skipLWS);
	putback(result);
	return result;
}

void ScannerMessage::pushNewBuffer(const char* buffer, size_t len)
{
	_scan.pushNewBuffer(buffer, len);
}

void ScannerMessage::putback(Token token)
{
	this->putback(token.value);
}

void ScannerMessage::putback(std::string str)
{
	std::string::reverse_iterator it = str.rbegin();
	std::string::reverse_iterator end = str.rend();
	while (it != end)
	{
		_scan.putback(*it);
		it++;
	}
}

/// Must only be called in the switch statement
bool ScannerMessage::_charIsString(char c){
	if (c == ':' || isspace(c))
		return false;
	if (isprint(c))
		return true;
	return false;
}

Token ScannerMessage::_makeToken(TokenKind kind, std::string value)
{
	Token t;

	t.kind = kind;
	t.value = value;
	return t;
}

std::ostream & operator <<(std::ostream& os, const Token &t)
{
	os << "<" << TokenKindToCstring(t.kind);
	switch (t.kind.getValue())
	{
		case (TokenKind::kString)		:	os << "=\"" << t.value << "\"> ";	break;
		case (TokenKind::kError)		:	os << "=\"" << t.value << "\"> ";	break;
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
	return str[type.getValue()];
}

} /* namespace http */
} /* namespace parser */
