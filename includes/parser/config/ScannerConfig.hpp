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
#include "parser/ScannerStream.hpp"

namespace parser{

namespace config{


struct TokenKind {

	enum TokenKindEnum
	{
		kEndOfInput = 0, kError,
		kString, kInteger, kComment,
		kLeftBrace, kRightBrace,
		kComma, kColon, kSemiColon,
		kNewLine,
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
// 		kString, kInteger, kComment,
// 		kLeftBrace, kRightBrace,
// 		kComma, kColon, kSemiColon,
// 		kNewLine,
// 	};
// };
// typedef ScopedEnum::TokenKind TokenKind;

struct Token
{
	Token(): kind(TokenKind::kEndOfInput) {};

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

	Token getToken(bool skipNL = false);

private:

	bool _charIsString(char c);
	Token _makeToken(TokenKind kind, std::string value);
	Token _makeToken(TokenKind kind, std::string value, int column, int line);

	ScannerStream _scan;
}; /* class ScannerConfig */

const char* tokenKindToCstring(TokenKind kind);
std::string tokenToString(Token token);
std::ostream & operator <<(std::ostream& os, const Token &t);

} /* namespace config */
} /* namespace parser */

#endif /* SCANNER_CONFIG_HPP */
