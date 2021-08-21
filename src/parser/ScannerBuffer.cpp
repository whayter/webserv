/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerBuffer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 01:06:24 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/21 17:30:18 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/ScannerBuffer.hpp"
#include <sys/socket.h>


parser::ScannerBuffer::ScannerBuffer(const char *buffer)
	: _line(1), _column(0), _c(0), _buffer(buffer), _idx(0) {}

parser::ScannerBuffer::~ScannerBuffer() {}

void parser::ScannerBuffer::moveForward()
{
	if (_c == '\n')
	{
		_line++;
		_lastColumn = _column;
		_column = 1;
	}
	else
		_column++;

	if (!_charsPutback.empty())
	{
		_c =  _charsPutback[_charsPutback.size() - 1];
		_charsPutback.erase(_charsPutback.size() - 1);
	}
	else
		_c = _buffer[_idx++];
}

void parser::ScannerBuffer::moveBackward(char charToPutBack)
{
	if (charToPutBack == '\n')
	{
		_line--;
		_column = _lastColumn;
	}
	else
		_column--;
	_charsPutback.push_back(charToPutBack);
}

char parser::ScannerBuffer::get()
{
	moveForward();
	return _c;
}

char parser::ScannerBuffer::putback(char c)
{
	moveBackward(c);
	return _c;
}
