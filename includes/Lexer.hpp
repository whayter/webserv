/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 21:45:59 by juligonz          #+#    #+#             */
/*   Updated: 2021/07/31 22:01:39 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <cctype>

struct ScopedEnumTokenType
{
	enum TokenType
	{
		kIdentifier = 0, 
		kString, kInteger,
		kLeftBrace, kRightBrace,
		kSemicolon
		kEof, kError 
	};
};
typedef ScopedEnumTokenType::TokenType TokenType;

/// @brief Used for type punning (c++98...), TokenValue can be either int or string.
/// To know which type is active, refer to TokenType.
/// If the type not integer, then the active type is int.
// union TokenValue
// {
// 	std::string str;
// 	int			integer;
// };

struct Token
{
	TokenType	type;
	std::string	valueString;
	int			valueInt;
};


#endif /* LEXER_HPP */