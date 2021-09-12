/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpMessage.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 13:20:58 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/05 15:56:21 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMessage.hpp"

AMessage::AMessage() {}
AMessage::~AMessage() {}

std::string& AMessage::getHeader(std::string name)
{
	return _headers[name];
}

void AMessage::setHeader(std::string name, std::string value)
{
	_headers[name] = value;
}

void AMessage::setContent(content_type content)
{
	_content = content;
}