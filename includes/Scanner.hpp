/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scanner.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 21:45:53 by juligonz          #+#    #+#             */
/*   Updated: 2021/07/31 21:47:00 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <istream>
#include <iostream>

class Scanner
{
public:
	Scanner (std::istream &inputStream) :
		_line(0), _column(0), _inStream(inputStream), _c(0) {}
	~Scanner(){};

	inline char	peek(void)		{ return _c; }
	inline int	getLine(void)	{ return _line; }
	inline int	getColumn(void)	{ return _column; }
	
	/// @brief Increment to next character, and increment line/column count 
	void moveForward()
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

	char next()
	{
		moveForward();
		return peek();
	}

	void skipWhitespaces()
	{
		while (isspace(static_cast<unsigned char>(peek())))
			moveForward();		
	}
private:
	Scanner();

	int _line;
	int _column;
	std::istream& _inStream;
	char _c;
}; /* struct Scanner */

#endif /* SCANNER_HPP */
