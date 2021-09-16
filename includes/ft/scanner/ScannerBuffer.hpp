/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerBuffer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 21:45:53 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/21 18:55:56 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_BUFFER_HPP
#define SCANNER_BUFFER_HPP

#include <deque>
#include <string>

namespace ft
{
	namespace scanner
	{

		class ScannerBuffer
		{
		public:
			ScannerBuffer();
			ScannerBuffer(const char *buffer);
			~ScannerBuffer();

			/// @brief Get the next character
			char get();
			/// @brief putback the given character.
			void putback(char c);

			// spaghetti fix
			void pushNewBuffer(const char *buffer, size_t len);

			std::string toString();

		private:
			char _c;

			std::deque<char> _buffer;
		}; /* class ScannerBuffer */

	} /* namespace scanner */
} /* namespace ft */

#endif /* SCANNER_BUFFER_HPP */