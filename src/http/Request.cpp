/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/15 14:47:13 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/15 14:49:48 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

namespace http {

Request::Request() {}

Request::~Request()
{
	_headers.clear();
	_content.clear();
}

void Request::setMethod(std::string method)
{
	_method = method;
}

void Request::setUri(const Uri& uri)
{
	_uri = uri;
}

} /* namespace http */