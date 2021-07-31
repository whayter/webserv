#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <cctype>
#include <string>
#include <istream>

class Scanner
{
public:
	// Scanner (const char *source) : pos(source) {}
	Scanner (std::ifstream *inputStream) : inStream(inputStream) {}
	~Scanner(){};
	inline char peek(){return ;}
	inline int getLine(void)	{ return line;}
	inline int getColumn(void)	{ return column;}
	
	/// @brief Increment to next character, and increment line/column count 
	void moveForward()
	{
		if (*pos == '\n')
		{
			line++;
			column = 1;
		}
		else
			column++;
		pos++;
	}

	char next()
	{
		moveForward();
		return peek();
	}

	// void skipWhitespaces()
	// {
	// 	while (isspace(static_cast<unsigned char>(peek())))
	// 		moveForward();		
	// }
private:
	Scanner();
	Scanner();

	int line	= 1;
	int column	= 1;
	// const char *pos;
	std::ifstream *inStream;

}; /* struct Scanner */

struct ScopedEnumTokenType
{
	enum class TokenType
	{
		IDENTIFIER, STRING, INTEGER
	};
};
typedef ScopedEnumTokenType::TokenType TokenType;

/// @brief Used for type punning (c++98...), TokenValue can be either int or string.
/// To know which type is active, refer to TokenType.
/// If the type not integer, then the active type is int.
union TokenValue
{
	std::string str;
	int			integer;
};

struct Token
{
	TokenType type;
	TokenValue value;
	int
};



#endif /* SCANNER_HPP */
