/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerHttpRequest.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 13:36:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/06 14:19:48 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_HTTP_REQUEST_HPP
#define SCANNER_HTTP_REQUEST_HPP

#include <istream>
#include <parser/Scanner.hpp>

namespace parser{
namespace http{

struct ScopedEnum
{
	enum TokenKind
	{
		kEndOfInput = 0, kError,
		kString, kNewLine,
		kLeftBrace, kRightBrace,
		kComma, kColon,
	};
};
typedef ScopedEnum::TokenKind TokenKind;
struct Token
{
	TokenKind	kind;
	std::string	valueString;
	// int			valueInt; 	// stupid union that doesn't work under
	// 						// c++98 with complex type like strings
	int			line;
	int			column;
};


class ScannerHttpRequest
{
public:
	ScannerHttpRequest(std::istream &inputStream);
	~ScannerHttpRequest();

	Token getToken();
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
