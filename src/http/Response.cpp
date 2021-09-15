/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/15 14:48:22 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/15 14:54:46 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

namespace http {

Response::Response() {}

Response::~Response()
{
	_headers.clear();
	_content.clear();
}

void Response::setStatus(Status::StatusEnum e)
{
	_status = e;
}

} /* namespace http */