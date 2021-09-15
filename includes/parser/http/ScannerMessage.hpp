/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerMessage.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 13:36:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/09/05 13:16:51 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_HTTP_REQUEST_HPP
#define SCANNER_HTTP_REQUEST_HPP

#include <istream>
#include <ft/scanner/ScannerBuffer.hpp>

namespace parser{
namespace http{

// LWS = " " | "\t"


struct TokenKind {

	enum TokenKindEnum
	{
		kEndOfInput = 0, kError,
		kString, kNewLine, kCarriage,
		kLeftBrace, kRightBrace,
		kComma, kColon, kLWS
	};

	TokenKind(TokenKindEnum e): _e(e) {}
	TokenKind(unsigned int e) {
		_e = static_cast<TokenKindEnum>(e);
	}
	// operator TokenKindEnum() const throw(){
	// 	return _e;
    // }

	int getValue() const { return _e; }

    friend bool operator==(const TokenKind& lhs, const TokenKind& rhs){
      return lhs._e == rhs._e;
    }
	 friend bool operator!=(const TokenKind& lhs, const TokenKind& rhs){
      return lhs._e != rhs._e;
    }

private:
	TokenKindEnum _e;
};



// struct ScopedEnum
// {
// 	enum TokenKind
// 	{
// 		kEndOfInput = 0, kError,
// 		kString, kNewLine, kCarriage,
// 		kLeftBrace, kRightBrace,
// 		kComma, kColon, kLWS
// 	};
// };
// typedef ScopedEnum::TokenKind TokenKind;

struct Token
{
	Token(): kind(TokenKind::kEndOfInput) {};

	TokenKind	kind;
	std::string	value;
};


class ScannerMessage
{
public:
	ScannerMessage(const char *buffer);
	~ScannerMessage();

	Token getToken(bool skipLWS = false);
	char getChar();
	Token peekNextToken(bool skipLWS = false);

	// spaghetti fix
	void pushNewBuffer(const char *buffer, size_t len);
	void putback(Token token);
	void putback(std::string str);

private:

	bool _charIsString(char c);
	Token _makeToken(TokenKind kind, std::string value);

	ft::scanner::ScannerBuffer _scan;
	Token	_nextToken;
}; /* class ScannerMessage */

const char* TokenKindToCstring(TokenKind type);
std::ostream & operator <<(std::ostream& os, const Token &t);

} /* namespace http */
} /* namespace parser */

#endif /* SCANNER_HTTP_REQUEST_HPP */
