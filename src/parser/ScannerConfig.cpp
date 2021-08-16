/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 13:39:02 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/16 16:43:09 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/config/ScannerConfig.hpp"

namespace parser
{
namespace config
{

ScannerConfig::ScannerConfig(std::istream &inputStream)
	: _scan(inputStream)
{}

ScannerConfig::~ScannerConfig(){}

Token ScannerConfig::getToken(bool skipNL)
{
	char c = 0;

	if (skipNL)
		do
		{
			c = _scan.get();
		} while (isspace(c));
	else
		do
		{
			c = _scan.get();
		} while (c != '\n' && isspace(c));
	
	switch (c)
	{
		case  -1:
		case  0 :	return _makeToken(ScopedEnum::kEndOfInput, "");
		// case ':':	return _makeToken(ScopedEnum::kColon, ":");
		// case ',':	return _makeToken(ScopedEnum::kComma, ",");
		case ';':	return _makeToken(ScopedEnum::kSemiColon, ";");
		case '{':	return _makeToken(ScopedEnum::kLeftBrace, "{");
		case '}':	return _makeToken(ScopedEnum::kRightBrace, "}");
		case '\n':	return _makeToken(ScopedEnum::kNewLine, "\n");
		default:
			std::string lexeme = "";
			int column = _scan.getColumn(), line = _scan.getLine();

			if (_charIsString(c))
			{
				while (_charIsString(c))
				{
					lexeme += c;
					c = _scan.get();
				}
				if (!_charIsString(c))
					_scan.unget();
				return _makeToken(ScopedEnum::kString, lexeme, column, line);
			}
			return _makeToken(ScopedEnum::kError,
						std::string("Cant parse lexeme:\"" + lexeme +"\", char:'" + c+ "'  "));
	}
}

/// Must only be called in the switch statement
bool ScannerConfig::_charIsString(char c){
	if (c == ';' || c == '{' || c == '}' || isspace(c))
		return false;
	if (isprint(c))
		return true;
	return false;
}

Token ScannerConfig::_makeToken(TokenKind kind, std::string value, int column, int line)
{
	Token t;

	t.kind = kind;
	t.value = value;
	t.column = column;
	t.line = line;
	return t;
}
Token ScannerConfig::_makeToken(TokenKind kind, std::string value)
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
		case (ScopedEnum::kString)		:
		case (ScopedEnum::kError)		:	os << "=\"" << t.value << "\"> ";	break;
		default							:	os << "> "; break;
	}
	return os;
}

const char* TokenKindToCstring(TokenKind type)
{
	static const char* str[] = {
		"kEnfOfInput", "kError",
		"kString",
		"kLeftBrace", "kRightBrace",
		"kComma", "kColon", "kSemiColon",
		"kNewLine",
		"kIdentifier"
	};
	return str[type];
}

} /* namespace config */

} /* namespace parser */
