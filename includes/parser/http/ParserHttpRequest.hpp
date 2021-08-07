/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserHttpRequest.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 14:55:33 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/07 11:24:47 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HTTP_REQUEST_HPP
#define PARSER_HTTP_REQUEST_HPP

#include "parser/http/ScannerHttpRequest.hpp"
#include "http/HttpRequest.hpp"


#include <iostream>

namespace parser
{
namespace http
{

class ParserHttpRequest
{
public:

	static HttpRequest create(std::istream inputStream){
		ScannerHttpRequest scanner(inputStream);
		HttpRequest result;
		Token t = scanner.getToken();
	// std::cout << t << std::endl; //////
		if (	!t.value.compare("GET") ||	!t.value.compare("POST")
			||	!t.value.compare("DELETE"))
		{
			result.setMethod(t.value);
			t = scanner.getToken();
	// std::cout << t << std::endl; //////
			result.setUri(Uri(t.value));
		}
		while((t = scanner.getToken()).kind != ScopedEnum::kEndOfInput)
		{
	// std::cout << t << std::endl; /////////
			if (t.kind == parser::http::ScopedEnum::kEndOfInput)
				break;
		}
		// std::cout << std::endl;
		
		return result;
	}
private:
	ParserHttpRequest();
	~ParserHttpRequest();

};

ParserHttpRequest::ParserHttpRequest(){}

ParserHttpRequest::~ParserHttpRequest(){}

} /* namespace http */
} /* namespace parser */

#endif /* PARSER_HTTP_REQUEST_HPP */
