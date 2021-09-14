/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Device.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 10:18:09 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/14 11:04:20 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Device.hpp"

#include <iostream>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>

namespace web {
    
void Device::setPort(uint32_t port)
{
    _port = port;
}

void Device::closeSocket()
{
	if (close(getSocket().getFd()) == -1)
		std::cerr << "Error close(): " << strerror(errno) << std::endl;
}

bool Device::setNonBlocking()
{
	if (fcntl(getSocket().getFd(), F_SETFL, O_NONBLOCK) != -1)
		return true;
	std::cerr << "Error fcntl(): " << strerror(errno) << std::endl;
	return false;
}

bool Device::setReusableAddr()
{
	int opt = 1;
	if (setsockopt(getSocket().getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == 0)
		return true;
	std::cerr << "Error setsockopt(): " << strerror(errno) << std::endl;
	return false;
}

bool Device::bindSocket()
{
    addr_type addr = getSocket().getAddr();
	if (bind(getSocket().getFd(), (struct sockaddr*)&addr, sizeof(addr)) == 0)
		return true;
	std::cerr << "Error bind(): " << strerror(errno) << std::endl;
	return false;
}

bool Device::listenSocket()
{
	if (listen(getSocket().getFd(), SOMAXCONN) == 0)
		return true;
	std::cerr << "Error listen(): " << strerror(errno) << std::endl;
	return false;
}

}; /* namespace web */