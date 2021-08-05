/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AScanner.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 01:06:24 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/05 15:05:22 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "parser/AScanner.hpp"

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
