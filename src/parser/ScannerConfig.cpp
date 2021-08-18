/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 13:39:02 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/16 18:29:50 by juligonz         ###   ########.fr       */
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
		case '#': 
		{
			std::string lexeme = "";
			int column = _scan.getColumn(), line = _scan.getLine();
		
			while (c != '\n')
			{
				lexeme += c;
				c = _scan.get();
			}
			if (c == '\n')
				_scan.unget();
			return _makeToken(ScopedEnum::kComment, lexeme, column, line);
		}
		default:
		{
			std::string lexeme = "";
			int column = _scan.getColumn(), line = _scan.getLine();
			bool isInteger = true;

			if (_charIsString(c))
			{
				while (_charIsString(c))
				{
					if (isInteger && !isdigit(c))
						isInteger = false;
					lexeme += c;
					c = _scan.get();
				}
				if (!_charIsString(c))
					_scan.unget();
				if (isInteger)
					return _makeToken(ScopedEnum::kInteger, lexeme, column, line);
				return _makeToken(ScopedEnum::kString, lexeme, column, line);
			}
			return _makeToken(ScopedEnum::kError,
						std::string("Cant parse lexeme:\"" + lexeme +"\", char:'" + c+ "'  "));
		}
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
	os << "<" << tokenKindToCstring(t.kind);
	switch (t.kind)
	{
		case (ScopedEnum::kString)		:
		case (ScopedEnum::kInteger)		:
		case (ScopedEnum::kError)		:	os << "=\"" << t.value << "\"> ";	break;
		default							:	os << "> "; break;
	}
	return os;
}

const char* tokenKindToCstring(TokenKind kind)
{
	static const char* str[] = {
		"kEnfOfInput", "kError",
		"kString", "kInteger", "kComment",
		"kLeftBrace", "kRightBrace",
		"kComma", "kColon", "kSemiColon",
		"kNewLine",
		"kIdentifier"
	};
	return str[kind];
}

std::string tokenToString(Token token)
{
	std::string result("<");
	
	result += tokenKindToCstring(token.kind);
	switch (token.kind)
	{
		case (ScopedEnum::kString)		:
		case (ScopedEnum::kError)		:
			result += "=\"" + token.value + "\"> ";
			break;
		default							:
			result += ">";
			break;
	}
	return result;
}

} /* namespace config */

} /* namespace parser */
