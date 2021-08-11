/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 11:47:10 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/11 12:28:03 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/server/Client.hpp"				// temporary path

/* --- Client class - Member functions -------------------------------------- */

clnt::Client::Client() {}

clnt::Client::~Client() {}

void	clnt::Client::setFd(sckt::fd_type fd)
{
	_socket.setFd(fd);
}

void	clnt::Client::setAddr(sckt::addr_type addr)
{
	_socket.setAddr(addr);
}

void	clnt::Client::setPfd(sckt::fd_type fd)
{
	_pfd.fd = _socket.getFd();
	_pfd.events = POLLIN;
}

/* --- clnt namespace functions --------------------------------------------- */

bool	clnt::connect(sckt::fd_type fd, sckt::addr_type addr)
{
	return connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == 0;
}

void	clnt::disconnect(const Client& client)
{
	sckt::closeSocket(client.getFd());
}
