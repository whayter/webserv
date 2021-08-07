/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserHttpRequest.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 14:55:33 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/07 17:29:17 by juligonz         ###   ########.fr       */
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

	static HttpRequest create(std::istream & inputStream){
		ScannerHttpRequest scanner(inputStream);
		HttpRequest result;
		Token t = scanner.getToken();
		if (	!t.value.compare("GET") ||	!t.value.compare("POST")
			||	!t.value.compare("DELETE"))
		{
			result.setMethod(t.value);
			std::cout << t << std::endl; /////////
			
			t = scanner.getToken();
			result.setUri(Uri(t.value));
			std::cout << t << std::endl; /////////
			
			t = scanner.getToken();
			result.setVersion(t.value);
			std::cout << t << std::endl; /////////
		}
		while((t = scanner.getToken()).kind != ScopedEnum::kEndOfInput)
		{
			std::cout << t << std::endl; /////////
			// switch (t.kind)
			// {
			// case ScopedEnum::k:
				
			// 	break;
			
			// default:
			// 	break;
			// }
		}
		std::cout << t << std::endl; /////////
		
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
