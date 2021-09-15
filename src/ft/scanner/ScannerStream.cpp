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

#include "ft/scanner/ScannerStream.hpp"

namespace ft{
namespace scanner{

ScannerStream::ScannerStream (std::istream &inputStream):
	_line(1), _column(0), _inStream(inputStream), _c(0) {}
ScannerStream::~ScannerStream(){};

void ScannerStream::moveForward()
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

void ScannerStream::moveBackward(char charToPutBack)
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

char ScannerStream::get()
{
	moveForward();
	return _c;
}

void ScannerStream::putback(char c)
{
	moveBackward(c);
}

} /* namespace scanner */
} /* namespace ft */
