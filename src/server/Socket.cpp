/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 11:26:26 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/20 00:51:06 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Socket.hpp"

/* --- Socket class - member functions -------------------------------------- */

sckt::Socket::Socket() {}

sckt::Socket::~Socket() {}

bool	sckt::Socket::setFd(int domain, int type)
{
	return (_fd = socket(domain, type, 0)) != INVALID_FD;
}

void	sckt::Socket::setFd(fd_type fd)
{
	_fd = fd;
}
					
void	sckt::Socket::setAddr(int domain, int interface, int port)
{
	_addr.sin_family = domain;
	_addr.sin_addr.s_addr = interface;
	_addr.sin_port = htons(port);
	memset(_addr.sin_zero, 0, sizeof(_addr.sin_zero));
	_addrLen = static_cast<socklen_t>(sizeof(_addr));
}
		
void	sckt::Socket::setAddr(const addr_type& addr)
{
	_addr = addr;
}

/* --- Namespace functions -------------------------------------------------- */

void	sckt::closeSocket(fd_type fd)
{
	if (fd != INVALID_FD)
		close(fd);
}

bool	sckt::setNonBlocking(fd_type fd)
{
	return fcntl(fd, F_SETFL, O_NONBLOCK) != -1;
}

bool	sckt::setReusableAddr(fd_type fd)
{
	int opt = 1;
	return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == 0;
}

bool	sckt::bindSocket(fd_type fd, const addr_type& addr)
{
	return bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == 0;
}

bool	sckt::listenSocket(fd_type fd)
{
	return listen(fd, SOMAXCONN) == 0;
}
