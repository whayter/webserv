/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerBuffer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 01:06:24 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/21 18:59:12 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScannerBuffer.hpp"

parser::ScannerBuffer::ScannerBuffer() : _c(0) {}

parser::ScannerBuffer::ScannerBuffer(const char *buffer)
	: _c(0)
{
	std::size_t i = 0;

	if (buffer)
		while (buffer[i])
			_buffer.push_back(buffer[i++]);
}

parser::ScannerBuffer::~ScannerBuffer() {}

char parser::ScannerBuffer::get()
{
	if (_buffer.begin() == _buffer.end())
		_c = 0;
	else
	{
		_c = _buffer.front();
		_buffer.pop_front();
	}
	return _c;
}

void parser::ScannerBuffer::putback(char c)
{
	_buffer.push_front(c);
}

void parser::ScannerBuffer::pushNewBuffer(const char *buffer, size_t len)
{
	std::size_t i = 0;

	while (len--)
		_buffer.push_back(buffer[i++]);
}

std::string parser::ScannerBuffer::toString()
{
	std::string result;
	std::deque<char>::iterator it = _buffer.begin();
	std::deque<char>::iterator end = _buffer.end();

	while (it != end)
	{
		result += *it;
		it++;
	}

	return result;
}
