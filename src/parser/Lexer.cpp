/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 01:10:59 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/05 15:05:31 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "parser/Lexer.hpp"

// Lexer::Lexer(std::istream inputStream):
// 	_scanner(inputStream), _pre_state(_scanner) {}

// bool Lexer::hasNext(void)
// {
// 	return _scanner.eof();
// }

// Token Lexer::makeToken(TokenType type, Token t)
// {
// 	if (type == ScopedEnum::kInteger)
// 		return ;
// 	return (Token){.type=type,.valueString=NULL,.valueInt=0,.line=_pre_state};
// }

// Lexer::~Lexer()
// {
// }