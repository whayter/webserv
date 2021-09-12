/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   web.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 22:45:42 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/11 00:19:11 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "web.hpp"

#include <iostream>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>

void	web::closeSocket(fd_type fd)
{
	if (close(fd) == -1)
		std::cerr << "Error close(): " << strerror(errno) << std::endl;
}

bool	web::setNonBlocking(fd_type fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) != -1)
		return true;
	std::cerr << "Error fcntl(): " << strerror(errno) << std::endl;
	return false;
}

bool	web::setReusableAddr(fd_type fd)
{
	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == 0)
		return true;
	std::cerr << "Error setsockopt(): " << strerror(errno) << std::endl;
	return false;
}

bool	web::bindSocket(fd_type fd, const addr_type& addr)
{
	if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == 0)
		return true;
	std::cerr << "Error bind(): " << strerror(errno) << std::endl;
	return false;
}

bool	web::listenSocket(fd_type fd)
{
	if (listen(fd, SOMAXCONN) == 0)
		return true;
	std::cerr << "Error listen(): " << strerror(errno) << std::endl;
	return false;
}