/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpMessage.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 13:20:58 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/05 12:39:53 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/AHttpMessage.hpp"

AHttpMessage::AHttpMessage() {}
AHttpMessage::~AHttpMessage() {}

std::string& AHttpMessage::getHeader(std::string name)
{
	return _headers[name];
}

void AHttpMessage::setHeader(std::string name, std::string value)
{
	_headers[name] = value;
}

void AHttpMessage::setContent(content_type content)
{
	_content = content;
}