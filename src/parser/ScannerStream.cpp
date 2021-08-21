/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerStream.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 01:06:24 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/21 17:30:23 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/ScannerStream.hpp"

parser::ScannerStream::ScannerStream (std::istream &inputStream):
	_line(1), _column(0), _inStream(inputStream), _c(0) {}
parser::ScannerStream::~ScannerStream(){};

void parser::ScannerStream::moveForward()
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

void parser::ScannerStream::moveBackward(char charToPutBack)
{
	if (charToPutBack == '\n')
	{
		_line--;
		_column = _lastColumn;
	}
	else
		_column--;
	_inStream.putback(charToPutBack);
}

char parser::ScannerStream::get()
{
	moveForward();
	return _c;
}

char parser::ScannerStream::putback(char c)
{
	moveBackward(c);
	return _c;
}
