/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserHttpRequest.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 14:55:33 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/06 17:38:31 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HTTP_REQUEST_HPP
#define PARSER_HTTP_REQUEST_HPP

#include "parser/http/ScannerHttpRequest.hpp"
#include "http/HttpRequest.hpp"

namespace parser
{
namespace http
{

class ParserHttpRequest
{
public:
	ParserHttpRequest(std::istream inputStream);
	~ParserHttpRequest();

private:
	static HttpRequest create(){
		
	}

	ScannerHttpRequest _scan;
};

ParserHttpRequest::ParserHttpRequest(std::istream inputStream)
	: _scan(inputStream)
{
	Token t;

	t = _scan.getToken();
	
}

ParserHttpRequest::~ParserHttpRequest(){}

} /* namespace http */
} /* namespace parser */

#endif /* PARSER_HTTP_REQUEST_HPP */
