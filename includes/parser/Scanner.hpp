/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scanner.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 21:45:53 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/06 12:11:22 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <istream>

// namespace lexer {
class Scanner
{
public:
	Scanner(std::istream &inputStream);
	~Scanner();

	/// Peek the next character
	inline char	peek(void)		{ return _inStream.peek(); }
	inline int	getLine(void)	{ return _line; }
	inline int	getColumn(void)	{ return _column; }
	inline bool	eof(void)		{ return _inStream.eof(); }
	
	/// @brief Get the next character
	char		get();
	/// @brief Decrease stream index by one character.
	char		unget();
	// void		skipWhitespaces();

private:
	Scanner();

	/// @brief Increment to next character, and increment line/column count 
	void		moveForward();

	int				_line;
	int				_column;
	std::istream&	_inStream;
	char			_c;
}; /* class Scanner */


///////////////////////
///////////////////////
Scanner::Scanner (std::istream &inputStream):
	_line(0), _column(0), _inStream(inputStream), _c(0) {}
Scanner::~Scanner(){};

void Scanner::moveForward()
{
	if (_c == '\n')
	{
		_line++;
		_column = 1;
	}
	else
		_column++;
	_c = _inStream.get();
}

char Scanner::get()
{
	moveForward();
	return _c;
}

// void Scanner::skipWhitespaces()
// {
// 	while (isspace(static_cast<unsigned char>(peek())))
// 		moveForward();		
// }

///////////////////////
///////////////////////
///////////////////////

struct ScopedEnum
{
	enum TokenKind
	{
		kEndOfInput = 0, kError,
		kString, kInteger,
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
	std::string	valueString;
	// int			valueInt; 	// stupid union that doesn't work under
	// 						// c++98 with complex type like strings
	int			line;
	int			column;
};

const char* TokenKindToCstring(TokenKind type)
{
	static const char* str[] = {
		"kEnfOfInput", "kError",
		"kString", "kInteger",
		"kLeftBrace", "kRightBrace",
		"kComma", "kColon",
		"kNewLine",
		"kIdentifier"
	};
	return str[type];
}


std::ostream & operator <<(std::ostream& os, const Token &t)
{
	os << "<" << TokenKindToCstring(t.kind);
	switch (t.kind)
	{
		case (ScopedEnum::kString)		:	os << "=\"" << t.valueString << "\"> ";	break;
		case (ScopedEnum::kError)		:	os << "=\"" << t.valueString << "\"> ";	break;
		// case (ScopedEnum::kInteger)		: 	os << ":" << t.valueInt;				break;
		
		default							:	os << "> "; break;
	}
	return os;
}

class Scanner2
{

public:
	Scanner2(std::istream &inputStream);
	~Scanner2();

	Token getToken();
	inline Token peekToken() { return _actualToken; };
private:

	bool charIsString(char c);
	Token makeToken(TokenKind kind, std::string value)
	{
		Token t;

		t.kind = kind;
		t.valueString = value;
		t.column = _scan.getColumn();
		t.line = _scan.getLine();
		return t;
	} 

	Scanner _scan;
	Token	_actualToken;
}; /* class Scanner */

//////////

Scanner2::Scanner2(std::istream &inputStream)
	: _scan(inputStream)
{}

Scanner2::~Scanner2(){}

Token Scanner2::getToken()
{
	char c = 0;

	while ((c = _scan.get()))
	{
		do
		{
			/* code */
		} while (c != '\n' && isspace(c));

		switch (c)
		{
			case  -1:
			case  0 :	return makeToken(ScopedEnum::kEndOfInput, "");
			case ':':	return makeToken(ScopedEnum::kColon, ":");
			case ',':	return makeToken(ScopedEnum::kComma, ",");
			case '\n':	return makeToken(ScopedEnum::kNewLine, ",");
			default:
				std::string lexeme = "";
				if (isalnum(c) || c == '/' || c == '.' )
				{
					while (isalnum(c) || c == '/' || c == '.' || c == '-')
					{
						lexeme+=c;
						c = _scan.get();
					}
					return makeToken(ScopedEnum::kString, lexeme);
				}
				return makeToken(ScopedEnum::kError, "Cant parse string");
		}	
	}
	return makeToken(ScopedEnum::kError, "Wtf man");
}

bool Scanner2::charIsString(char c){
	if (isalnum(c) || c == '/' || c == '.' || c == '-' || c==)
		return true;
	return false;
}

#endif /* SCANNER_HPP */
