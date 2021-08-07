/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AScanner.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 06:34:48 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/06 14:57:38 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASCANNER_HPP
#define ASCANNER_HPP

#include <istream>

// class AScanner
// {
// public:
// 	AScanner(std::istream &inputStream);
// 	virtual ~AScanner();

// protected:
// 	inline char		peek(void)		{ return _c; }
// 	inline int		getLine(void)	{ return _line; }
// 	inline int		getColumn(void)	{ return _column; }
// 	inline bool		eof(void)		{ return _inStream.eof(); }
	
// 	/// @brief Increment to next character, and increment line/column count 
// 	void			moveForward();
// 	/// @brief same as function moveForward(), but return the next character
// 	char			next();
// 	// void			skipWhitespaces();

// private:
// 	AScanner();

// 	int				_line;
// 	int				_column;
// 	std::istream&	_inStream;
// 	char			_c;
// }; /* class Scanner */


// AScanner::AScanner (std::istream &inputStream):
// 	_line(0), _column(0), _inStream(inputStream), _c(0) {}
// AScanner::~AScanner(){};

// void AScanner::moveForward()
// {
// 	if (_c == '\n')
// 	{
// 		_line++;
// 		_column = 1;
// 	}
// 	else
// 		_column++;
// 	_c = _inStream.get();
// }

// char AScanner::next()
// {
// 	moveForward();
// 	return peek();
// }

// // void AScanner::skipWhitespaces()
// // {
// // 	while (isspace(static_cast<unsigned char>(peek())))
// // 		moveForward();		
// // }


#endif /* ASCANNER_HPP */
