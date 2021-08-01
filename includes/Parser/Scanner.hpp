/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scanner.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 21:45:53 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/01 01:49:09 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <istream>

class Scanner
{
public:
	Scanner(std::istream &inputStream);
	~Scanner();

	inline char	peek(void)		{ return _c; }
	inline int	getLine(void)	{ return _line; }
	inline int	getColumn(void)	{ return _column; }
	
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
