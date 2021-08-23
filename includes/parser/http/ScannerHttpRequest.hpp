/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerHttpRequest.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 13:36:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/21 18:54:22 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_HTTP_REQUEST_HPP
#define SCANNER_HTTP_REQUEST_HPP

#include <istream>
#include <parser/ScannerBuffer.hpp>

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
};


class ScannerHttpRequest
{
public:
	ScannerHttpRequest(const char *buffer);
	~ScannerHttpRequest();

	Token getToken(bool skipLWS = false);
	char getChar();
	inline Token peekToken() { return _actualToken; };

	// spaghetti fix
	void pushNewBuffer(const char *buffer);
private:

	bool _charIsString(char c);
	Token _makeToken(TokenKind kind, std::string value);

	ScannerBuffer _scan;
	Token	_actualToken;
}; /* class ScannerHttpRequest */

const char* TokenKindToCstring(TokenKind type);
std::ostream & operator <<(std::ostream& os, const Token &t);

} /* namespace http */
} /* namespace parser */

#endif /* SCANNER_HTTP_REQUEST_HPP */
