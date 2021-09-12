/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 16:38:15 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/11 00:21:32 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

#include <string>
#include <arpa/inet.h>

web::Socket::Socket() {}

web::Socket::~Socket() {}

std::string	web::Socket::getIp()
{
	std::string ip(inet_ntoa(_addr.sin_addr));
	return ip;
}

bool web::Socket::setFd(int domain, int type)
{
	return (_fd = socket(domain, type, 0)) != INVALID_FD;
}

void web::Socket::setFd(fd_type fd)
{
	_fd = fd;
}
					
void web::Socket::setAddr(int domain, int interface, int port)
{
	_addr.sin_family = domain;
	_addr.sin_addr.s_addr = interface;
	_addr.sin_port = htons(port);
	memset(_addr.sin_zero, 0, sizeof(_addr.sin_zero));
	_addrLen = static_cast<addrLen_type>(sizeof(_addr));
}
		
void web::Socket::setAddr(const addr_type& addr)
{
	_addr = addr;
}