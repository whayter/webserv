/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerConfig.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 13:36:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/21 19:06:58 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_CONFIG_HPP
#define SCANNER_CONFIG_HPP

#include <istream>
#include "parser/IScanner.hpp"
#include "parser/ScannerStream.hpp"
#include "parser/ScannerBuffer.hpp"

namespace parser{

namespace config{

struct ScopedEnum
{
	enum TokenKind
	{
		kEndOfInput = 0, kError,
		kString, kInteger, kComment,
		kLeftBrace, kRightBrace,
		kComma, kColon, kSemiColon,
		kNewLine,
		kIdentifier
	};
};
typedef ScopedEnum::TokenKind TokenKind;

struct Token
{
	TokenKind	kind;
	std::string	value;
	int			line;
	int			column;
};


class ScannerConfig
{
public:
	ScannerConfig(std::istream &inputStream);
	ScannerConfig(const char *buffer);
	~ScannerConfig();

	Token getToken(bool skipNL = false);
	// inline Token peekToken() { return _actualToken; };

private:

	bool _charIsString(char c);
	Token _makeToken(TokenKind kind, std::string value);
	Token _makeToken(TokenKind kind, std::string value, int column, int line);

	IScanner* _scan;
	std::string	_remainingChars;
}; /* class ScannerConfig */

const char* tokenKindToCstring(TokenKind kind);
std::string tokenToString(Token token);
std::ostream & operator <<(std::ostream& os, const Token &t);

} /* namespace config */
} /* namespace parser */

#endif /* SCANNER_CONFIG_HPP */
