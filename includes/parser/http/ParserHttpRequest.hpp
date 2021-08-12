/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserHttpRequest.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 14:55:33 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/12 23:15:30 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HTTP_REQUEST_HPP
#define PARSER_HTTP_REQUEST_HPP

#include "parser/http/ScannerHttpRequest.hpp"
#include "http/HttpRequest.hpp"


#include <iostream>
#include <stdexcept>

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
		t = scanner.getToken();
		std::cout << "||||"<< t << std::endl;
		if (ScopedEnum::kNewLine != t.kind)
			throw std::invalid_argument("GET line not separated by new line");
		while((t = scanner.getToken()).kind != ScopedEnum::kEndOfInput)
		{
			std::string name;
			std::string value;
			bool isValueField = false;

			std::cout << t << std::endl;
			switch (t.kind)
			{
				case ScopedEnum::kNewLine :
					if (!name.empty())
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
					throw "Ho shit";
					break;
			}
		}
			std::cout << t << std::endl;
		
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
