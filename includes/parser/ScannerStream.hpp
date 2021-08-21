/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerStream.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 21:45:53 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/21 16:36:40 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_STREAM_HPP
#define SCANNER_STREAM_HPP

#include <istream>
#include "IScanner.hpp"

namespace parser {

class ScannerStream:  public IScanner
{
public:
	ScannerStream(std::istream &inputStream);
	~ScannerStream();

	/// Peek the next character
	// inline char	peek(void)		{ return _inStream.peek(); }
	inline int	getLine(void)	{ return _line; }
	inline int	getColumn(void)	{ return _column; }
	inline bool	eof(void)		{ return _inStream.eof(); }
	
	/// @brief Get the next character
	char		get();
	/// @brief Decrease stream index by one character.
	char		putback(char c);

private:
	ScannerStream();

	/// @brief Increment to next character, and increment line/column count 
	void		moveForward();
	/// @brief Decrement to previous character, and decrement line/column count 
	void		moveBackward(char charToPutBack);

	int				_line;
	int				_column;
	int				_lastColumn;
	std::istream&	_inStream;
	char			_c;
}; /* class ScannerStream */

} /* namespace parser */

#endif /* SCANNER_STREAM_HPP */
