/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerConfig.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 13:36:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/14 17:05:35 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_CONFIG_HPP
#define SCANNER_CONFIG_HPP

#include <istream>
#include <parser/Scanner.hpp>

namespace parser{

struct ScopedEnum
{
	enum TokenKind
	{
		kEndOfInput = 0, kError,
		kString,
		kLeftBrace, kRightBrace,
		kComma, kColon,
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
	~ScannerConfig();

	Token getToken();
	inline Token peekToken() { return _actualToken; };

private:

	bool _charIsString(char c);
	Token _makeToken(TokenKind kind, std::string value);
	Token _makeToken(TokenKind kind, std::string value, int column, int line);

	Scanner _scan;
	Token	_actualToken;
}; /* class ScannerConfig */

const char* TokenKindToCstring(TokenKind type);
std::ostream & operator <<(std::ostream& os, const Token &t);

} /* namespace parser */

#endif /* SCANNER_CONFIG_HPP */
