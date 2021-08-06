/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserHttpRequest.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 14:55:33 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/06 15:05:23 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HTTP_REQUEST_HPP
#define PARSER_HTTP_REQUEST_HPP

namespace parser
{
namespace http
{

class ParserHttpRequest
{
public:
	ParserHttpRequest(/* args */);
	~ParserHttpRequest();
private:

};

ParserHttpRequest::ParserHttpRequest(std::istream input str)
{
}

ParserHttpRequest::~ParserHttpRequest()
{
}


#endif
