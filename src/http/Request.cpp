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

bool Request::empty() const
{
	if (_headers.empty() && _content.empty() && _version.empty()
		&& _method.empty() && _uri.empty())
		return true;
	return false;
}

void Request::clear()
{
	_headers.clear();
	_content.clear();
	_version.clear();
	_method.clear();
	_uri.clear();
}

} /* namespace http */