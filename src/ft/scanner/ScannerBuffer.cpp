#include "ScannerBuffer.hpp"
#include <stdexcept>

namespace ft {

namespace scanner {

ScannerBuffer::ScannerBuffer(std::vector<unsigned char> &buffer)
	: _c(0), _idx(0), _buffer(buffer)
{}

ScannerBuffer::~ScannerBuffer() {}

unsigned char ScannerBuffer::get()
{
	if (_idx < _buffer.size())
		_c = _buffer[_idx++];
	else if (_idx == _buffer.size())
	{
		++_idx;
		_c = 0;
	}
	else
		throw std::out_of_range("Cant ScannerBuffer::get() overflow !");
	return _c;
}

void ScannerBuffer::unget()
{
	if (!_idx)
		throw std::out_of_range("Cant ScannerBuffer::unget() Underflow ! Already at index 0...");
	--_idx;
}

void ScannerBuffer::eraseBeforeCurrentIndex()
{
	_buffer.erase(_buffer.begin(), _buffer.begin() + _idx);
}


std::string ScannerBuffer::toString()
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
