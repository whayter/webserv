/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BindSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 22:43:30 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/04 00:11:29 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BINDSOCKET_H
#define BINDSOCKET_H

#include "ASocket.hpp"

class BindSocket: public ASocket
{
public:
	BindSocket(int family, int port, u_long interface, int service, int protocol);
	virtual ~BindSocket();

	virtual int connectToNetwork(int, sockaddr_in);

	int startListen(int socket, int backlog);

private:
	BindSocket(const BindSocket &);
	BindSocket & operator=(const BindSocket &);

	int _listening; // Negative if error
	int _backlog;	// 
};

#endif /* BINDSOCKET_HPP */

BindSocket::BindSocket(int family, int port, u_long interface, int service,
	int protocol): ASocket(family, port, interface, service, protocol)
{
	setConnection(
		connectToNetwork(getSocket(), getAddressIn())
	);		
}

int BindSocket::startListen(int socket, int backlog)
{
	_backlog = backlog;
	_listening = listen(getConnection(), backlog);
	testConnection(_listening);
}

int BindSocket::connectToNetwork(int sock, sockaddr_in address)
{
	return bind(sock, reinterpret_cast<sockaddr*>(&address), sizeof(address));
}

BindSocket::~BindSocket(){}
