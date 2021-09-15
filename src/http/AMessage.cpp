/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessage.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 18:55:56 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/15 14:48:39 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AMessage.hpp"

namespace http {

AMessage::AMessage() {}

std::string	AMessage::getHeader(std::string name)
{
	return _headers[name];
}

void AMessage::setHeader(std::string key, std::string val)
{
	_headers[key] = val;
}

void AMessage::setContent(content_type content)
{
	_content = content;
}

} /* namespace http */