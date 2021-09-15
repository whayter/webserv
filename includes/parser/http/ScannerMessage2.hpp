#ifndef SCANNER_MESSAGE_2_HPP
#define SCANNER_MESSAGE_2_HPP

#include <istream>
#include <ft/scanner/ScannerBuffer2.hpp>

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


class ScannerMessage2
{
public:
	ScannerMessage2(std::vector<unsigned char> &buffer);
	~ScannerMessage2();

	Token getToken(bool skipLWS = false);
	char getChar();
	// Token peekNextToken(bool skipLWS = false);

	// spaghetti fix
	// void pushNewBuffer(const char *buffer, size_t len);
	// void putback(Token token);
	// void putback(std::string str);

private:
	ScannerMessage2();

	bool _charIsString(char c);
	Token _makeToken(TokenKind kind, std::string value);

	ft::scanner::ScannerBuffer2 _scan;
}; /* class ScannerMessage2 */

const char* TokenKindToCstring(TokenKind type);
std::ostream & operator<<(std::ostream& os, const Token &t);

} /* namespace http */
} /* namespace parser */

#endif /* SCANNER_MESSAGE_2_HPP */
