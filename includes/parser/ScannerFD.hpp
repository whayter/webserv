/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScannerFD.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 21:45:53 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/13 18:19:27 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_FD_HPP
#define SCANNER_FD_HPP

#include <istream>
#include "IScanner.hpp"

namespace parser {

class ScannerFD:  public IScanner
{
public:
	ScannerFD(int fd);
	~ScannerFD();

	inline int	getLine(void)	{ return _line; }
	inline int	getColumn(void)	{ return _column; }

	/// Peek the next character
	char		peek(void);
	bool		eof(void);

	/// @brief Get the next character
	char		get();
	/// @brief Decrease stream index by one character.
	char		unget();



	static const size_t bufferSize = 500;
private:
	ScannerFD();

	/// @brief Increment to next character, and increment line/column count 
	void		moveForward();
	/// @brief Decrement to previous character, and decrement line/column count 
	void		moveBackward();


	int				_line;
	int				_column;
	int				_lastColumn;
	char			_c;
	
	char 			_buffer[bufferSize + 1];
	size_t			_idx;
	int				_fd;
}; /* class ScannerFD */

} /* namespace parser */

#endif /* SCANNER_FD_HPP */
