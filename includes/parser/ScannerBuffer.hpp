/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerBuffer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 21:45:53 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/21 17:08:42 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_BUFFER_HPP
#define SCANNER_BUFFER_HPP

#include <istream>
#include "IScanner.hpp"

namespace parser {

class ScannerBuffer:  public IScanner
{
public:
	ScannerBuffer(const char *buffer);
	~ScannerBuffer();

	inline int	getLine(void)	{ return _line; }
	inline int	getColumn(void)	{ return _column; }

	/// Peek the next character
	// char		peek(void);
	// bool		eof(void);

	/// @brief Get the next character
	char		get();
	/// @brief putback the given character.
	char		putback(char c);

private:
	ScannerBuffer();

	/// @brief Increment to next character, and increment line/column count 
	void		moveForward();
	/// @brief putback the giver character, and decrement line/column count 
	void		moveBackward(char c);


	int				_line;
	int				_column;
	int				_lastColumn;
	char			_c;
	
	const char*		_buffer;
	size_t			_idx;
	std::string		_charsPutback;
}; /* class ScannerBuffer */

} /* namespace parser */

#endif /* SCANNER_FD_HPP */
