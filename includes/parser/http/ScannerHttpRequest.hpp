/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerHttpRequest.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 13:36:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/14 17:05:35 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_HTTP_REQUEST_HPP
#define SCANNER_HTTP_REQUEST_HPP

#include <istream>
#include <parser/Scanner.hpp>

namespace parser{
namespace http{

// LWS = " " | "\t"

struct ScopedEnum
{
	enum TokenKind
	{
		kEndOfInput = 0, kError,
		kString, kNewLine, kCarriage,
		kLeftBrace, kRightBrace,
		kComma, kColon, kLWS
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


class ScannerHttpRequest
{
public:
	ScannerHttpRequest(std::istream &inputStream);
	~ScannerHttpRequest();

	Token getToken(bool skipLWS = false);
	char getChar();
	inline Token peekToken() { return _actualToken; };
private:

	bool _charIsString(char c);
	Token _makeToken(TokenKind kind, std::string value);

	Scanner _scan;
	Token	_actualToken;
}; /* class Scanner */

const char* TokenKindToCstring(TokenKind type);
std::ostream & operator <<(std::ostream& os, const Token &t);

} /* namespace http */
} /* namespace parser */

#endif /* SCANNER_HTTP_REQUEST_HPP */
