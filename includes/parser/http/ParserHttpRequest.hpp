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
		Token t = scanner.getToken(true);
		if (	!t.value.compare("GET") ||	!t.value.compare("POST")
			||	!t.value.compare("DELETE"))
		{
			result.setMethod(t.value);
			
			t = scanner.getToken(true);
			result.setUri(Uri("http", t.value));
			
			t = scanner.getToken(true);
			result.setVersion(t.value);
		}
		while((t = scanner.getToken()).kind != ScopedEnum::kEndOfInput)
		{
			std::string name;
			std::string value;
			bool isValueField = false;

			if (ScopedEnum::kNewLine  == t.kind)
				t = scanner.getToken();
			std::cout << t << std::endl;
			switch (t.kind)
			{
				case ScopedEnum::kNewLine :
					result.addHeader(name, value);
					name.clear();
					value.clear();
					isValueField = false;
					break;
				case ScopedEnum::kColon :
					isValueField = true;
					break;
				case ScopedEnum::kLWS :
					if (isValueField)
						value += t.value;
					break;
				case ScopedEnum::kString :
					if (isValueField == false)
						name += t.value;
					else
						value += t.value;
					break;
				
				default:
					break;
			}
		}
		result.addHeader("Cookie", "ASPSESSIONIDQADTQAQR=JNJLAIGBPIMBDAJPJNIFKIEK");
		
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
