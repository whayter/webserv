/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/01 01:10:59 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/01 01:39:12 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"

Lexer::Lexer(std::istream inputStream):
	_scanner(inputStream), _pre_state(_scanner)
{
	
}

Lexer::~Lexer()
{
}