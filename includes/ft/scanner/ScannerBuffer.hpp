#ifndef SCANNER_BUFFER_HPP
#define SCANNER_BUFFER_HPP

#include <vector>
#include <string>

namespace ft {
	
namespace scanner {

class ScannerBuffer
{
public:

	ScannerBuffer(std::vector<unsigned char> &buffer);
	~ScannerBuffer();

	/// @brief Get the next character
	unsigned char get();
	/// @brief Unget the last character.
	void unget();

	void eraseBeforeCurrentIndex();

	std::string toString();

private:

	ScannerBuffer();

	unsigned char	_c;
	size_t			_idx;

	std::vector<unsigned char>& _buffer;

}; /* class ScannerBuffer */

} /* namespace scanner */

} /* namespace ft */

#endif /* SCANNER_BUFFER_HPP */
