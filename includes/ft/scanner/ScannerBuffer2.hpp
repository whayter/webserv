#ifndef SCANNER_BUFFER2_HPP
#define SCANNER_BUFFER2_HPP

#include <vector>
#include <string>

namespace ft
{
	namespace scanner
	{

		class ScannerBuffer2
		{
		public:
			ScannerBuffer2(std::vector<unsigned char> &buffer);
			~ScannerBuffer2();

			/// @brief Get the next character
			unsigned char get();
			/// @brief Unget the last character.
			void unget();

			std::string toString();

		private:
			ScannerBuffer2();

			unsigned char	_c;
			size_t			_idx;

			std::vector<unsigned char>& _buffer;

		}; /* class ScannerBuffer2 */

	} /* namespace scanner */
} /* namespace ft */

#endif /* SCANNER_BUFFER2_HPP */
