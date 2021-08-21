/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scanner.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 01:06:24 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/21 15:25:58 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/Scanner.hpp"

parser::Scanner::Scanner (std::istream &inputStream):
	_line(1), _column(0), _inStream(inputStream), _c(0) {}
parser::Scanner::~Scanner(){};

void parser::Scanner::moveForward()
{
	if (_c == '\n')
	{
		_line++;
		_lastColumn = _column;
		_column = 1;
	}
	else
		_column++;
	_c = _inStream.get();
}

void parser::Scanner::moveBackward(char charToPutBack)
{
	if (_c == '\n')
	{
		_line--;
		_column = _lastColumn;
	}
	else
		_column--;
	_inStream.putback(charToPutBack);
}

char parser::Scanner::get()
{
	moveForward();
	return _c;
}

char parser::Scanner::putback(char c)
{
	moveBackward(c);
	return _c;
}
