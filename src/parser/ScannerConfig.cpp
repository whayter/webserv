/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 13:39:02 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/21 19:06:03 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/config/ScannerConfig.hpp"

namespace parser
{
namespace config
{

ScannerConfig::ScannerConfig(std::istream &inputStream)
	: _scan(inputStream)
{
	// _scan = new	ScannerStream(inputStream);
}

// ScannerConfig::ScannerConfig(const char *buffer)
// {
// 	_scan = new ScannerBuffer(buffer);
// }

ScannerConfig::~ScannerConfig() {}

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
		case  0 :	return _makeToken(TokenKind::kEndOfInput, "");
		// case ':':	return _makeToken(TokenKind::kColon, ":");
		// case ',':	return _makeToken(TokenKind::kComma, ",");
		case ';':	return _makeToken(TokenKind::kSemiColon, ";");
		case '{':	return _makeToken(TokenKind::kLeftBrace, "{");
		case '}':	return _makeToken(TokenKind::kRightBrace, "}");
		case '\n':	return _makeToken(TokenKind::kNewLine, "\n");
		case '#': 
		{
			std::string lexeme = "";
			int column = _scan.getColumn(), line = _scan.getLine();
		
			while (c != '\n' && c != -1 && c != 0)
			{
				lexeme += c;
				c = _scan.get();
			}
			if (c == '\n')
				_scan.putback(c);
			return _makeToken(TokenKind::kComment, lexeme, column, line);
		}
		case '\'':
		case '"':
		{
			std::string lexeme = "";
			int column = _scan.getColumn(), line = _scan.getLine();
			char quoteType = c;

			c = _scan.get();
			while (c != quoteType && c != '\n' && isprint(c))
			{
				lexeme += c;
				c = _scan.get();
			}
			if (c == '\n' || c == 0 || c== -1)
				return _makeToken(TokenKind::kError,
					std::string("Missing closing quote."), column, line);
			else if (!isprint(c))
				return _makeToken(TokenKind::kError,
					std::string("Wtf, characters that are not printable."), column, line);
			return _makeToken(TokenKind::kString, lexeme, column, line);
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
					_scan.putback(c);
				if (isInteger)
					return _makeToken(TokenKind::kInteger, lexeme, column, line);
				return _makeToken(TokenKind::kString, lexeme, column, line);
			}
			return _makeToken(TokenKind::kError,
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
	switch (t.kind.getValue())
	{
		case (TokenKind::kString)		:
		case (TokenKind::kInteger)		:
		case (TokenKind::kError)		:	os << "=\"" << t.value << "\"> ";	break;
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
	};
	return str[kind.getValue()];
}

std::string tokenToString(Token token)
{
	std::string result("<");
	
	result += tokenKindToCstring(token.kind);
	switch (token.kind.getValue())
	{
		case (TokenKind::kString)		:
		case (TokenKind::kError)		:
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
