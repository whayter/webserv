/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AScanner.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 06:34:48 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/03 06:36:45 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <istream>

class AScanner
{
public:
	AScanner(std::istream &inputStream);
	virtual ~AScanner();

	 char	peek(void)		{ return _c; }
	virtual int		getLine(void)	{ return _line; }
	virtual int		getColumn(void)	{ return _column; }
	virtual bool	eof(void)		{ return _inStream.eof(); }
	
	/// @brief Increment to next character, and increment line/column count 
	void		moveForward();
	/// @brief same as function moveForward(), but return the next character
	char		next();
	void		skipWhitespaces();

private:
	Scanner();

	int				_line;
	int				_column;
	std::istream&	_inStream;
	char			_c;
}; /* class Scanner */

#endif /* SCANNER_HPP */
