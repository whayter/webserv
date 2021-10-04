#ifndef SCANNER_MESSAGE_HPP
#define SCANNER_MESSAGE_HPP

#include <istream>
#include <ScannerBuffer.hpp>

namespace parser {

namespace http {

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

}; /* struct TokenKind */

struct Token
{
	Token(): kind(TokenKind::kEndOfInput) {};

	TokenKind	kind;
	std::string	value;
}; /* struct Token */

class ScannerMessage {

public:

	ScannerMessage(std::vector<unsigned char> &buffer);
	~ScannerMessage();

	Token getToken(bool skipLWS = false);
	unsigned char getChar();
	// Token peekNextToken(bool skipLWS = false);

	void eraseBeforeCurrentIndex();

	// spaghetti fix
	// void pushNewBuffer(const char *buffer, size_t len);
	// void putback(Token token);
	// void putback(std::string str);

private:

	ScannerMessage();

	bool _charIsString(char c);
	Token _makeToken(TokenKind kind, std::string value);
	ft::scanner::ScannerBuffer _scan;

}; /* class ScannerMessage */

const char* tokenKindToCstring(TokenKind type);
std::string tokenToString(Token token);
std::ostream & operator<<(std::ostream& os, const Token &t);

} /* namespace http */

} /* namespace parser */

#endif /* SCANNER_MESSAGE_HPP */
