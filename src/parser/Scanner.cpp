/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scanner.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 01:06:24 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/01 04:27:25 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/Scanner.hpp"

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

char Scanner::next()
{
	moveForward();
	return peek();
}

void Scanner::skipWhitespaces()
{
	while (isspace(static_cast<unsigned char>(peek())))
		moveForward();		
}
