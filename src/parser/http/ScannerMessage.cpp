#include "ScannerMessage.hpp"

namespace parser {

namespace http {

ScannerMessage::ScannerMessage(std::vector<unsigned char> &buffer)
	: _scan(buffer)
{}

ScannerMessage::~ScannerMessage() {}

Token ScannerMessage::getToken(bool skipLWS)
{
	char c = 0;

	if (skipLWS)
		do
		{
			c = _scan.get();
		} while (c != '\n' && c != '\r' && isspace(c));
	else
		c = _scan.get();
	
	switch (c)
	{
		case  -1:
		case  0 :	return _makeToken(TokenKind::kEndOfInput, "");
		case ':':	return _makeToken(TokenKind::kColon, ":");
		case ',':	return _makeToken(TokenKind::kComma, ",");
		case '\n':	return _makeToken(TokenKind::kNewLine, "\n");
		case '\r':	return _makeToken(TokenKind::kCarriage, "\r");
		case ' ':	return _makeToken(TokenKind::kLWS, " ");
		case '\t':	return _makeToken(TokenKind::kLWS, "\t");
		default:
			std::string lexeme = "";
			if (_charIsString(c))
			{
				while (_charIsString(c))
				{
					lexeme += c;
					c = _scan.get();
				}
				if (!_charIsString(c))
					_scan.unget();
				return _makeToken(TokenKind::kString, lexeme);
			}
			return _makeToken(TokenKind::kError,
						std::string("Cant parse lexeme:\"" + lexeme +"\", char:'" + c+ "'  "));
	}
}

unsigned char ScannerMessage::getChar()
{
	return _scan.get();
}

void ScannerMessage::eraseBeforeCurrentIndex()
{
	_scan.eraseBeforeCurrentIndex();
}

/// Must only be called in the switch statement
bool ScannerMessage::_charIsString(char c){
	if (c == ':' || isspace(c))
		return false;
	if (isprint(c))
		return true;
	return false;
}

Token ScannerMessage::_makeToken(TokenKind kind, std::string value)
{
	Token t;

	t.kind = kind;
	t.value = value;
	return t;
}

std::ostream & operator <<(std::ostream& os, const Token &t)
{
	os << "<" << TokenKindToCstring(t.kind);
	switch (t.kind.getValue())
	{
		case (TokenKind::kString)		:	os << "=\"" << t.value << "\"> ";	break;
		case (TokenKind::kError)		:	os << "=\"" << t.value << "\"> ";	break;
		default							:	os << "> "; break;
	}
	return os;
}

const char* TokenKindToCstring(TokenKind type)
{
	static const char* str[] = {
		"kEnfOfInput", "kError",
		"kString", "kNewLine", "kCarriage",
		"kLeftBrace", "kRightBrace",
		"kComma", "kColon", "kLWS"
	};
	return str[type.getValue()];
}

} /* namespace http */
} /* namespace parser */
