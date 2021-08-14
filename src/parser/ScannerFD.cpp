/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerFD.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 01:06:24 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/14 12:50:04 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/ScannerFD.hpp"
#include <sys/socket.h>


parser::ScannerFD::ScannerFD(int fd)
	: _line(0), _column(0), _fd(fd), _idx(0), _c(0)
{
	recv(_fd, &_buffer, bufferSize, 0);
}

parser::ScannerFD::~ScannerFD(){};

void parser::ScannerFD::moveForward()
{
	if (_c == '\n')
	{
		_line++;
		_lastColumn = _column;
		_column = 1;
	}
	else
		_column++;
	if (_idx >= bufferSize)
	{
		recv(_fd, &_buffer, bufferSize, 0);
		_idx = 0;
	}
	else
		_c = _buffer[_idx++];
	// _c = _inStream.get();
}

void parser::ScannerFD::moveBackward()
{
	if (_c == '\n')
	{
		_line--;
		_column = _lastColumn;
	}
	else
		_column--;
	_idx--;
	// _inStream.unget();
}

char parser::ScannerFD::get()
{
	moveForward();
	return _c;
}

char parser::ScannerFD::unget()
{
	moveBackward();
	return _c;
}
