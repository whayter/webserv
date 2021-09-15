/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerBuffer2.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 01:06:24 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/21 18:59:12 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScannerBuffer2.hpp"
#include <stdexcept>

namespace ft {
namespace scanner {

ScannerBuffer2::ScannerBuffer2(std::vector<unsigned char> &buffer)
	: _c(0), _idx(0), _buffer(buffer)
{}

ScannerBuffer2::~ScannerBuffer2() {}

unsigned char ScannerBuffer2::get()
{
	if (_idx < _buffer.size())
		_c = _buffer[_idx++];
	else if (_idx == _buffer.size())
	{
		++_idx;
		_c = 0;
	}
	else
		throw std::out_of_range("Cant ScannerBuffer::unget() overflow !");
	return _c;
}

void ScannerBuffer2::unget()
{
	if (!_idx)
		throw std::out_of_range("Cant ScannerBuffer::unget() Underflow ! Already at index 0...");
	--_idx;
}


std::string ScannerBuffer2::toString()
{
	std::string result;
	std::vector<unsigned char>::iterator it = _buffer.begin();
	std::vector<unsigned char>::iterator end = _buffer.end();

	while (it != end)
	{
		result += *it;
		it++;
	}

	return result;
}


} /* namespace scanner */
} /* namespace ft */
