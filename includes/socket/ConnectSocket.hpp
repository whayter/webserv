/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 00:15:39 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/04 10:34:34 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTSOCKET_HPP
#define CONNECTSOCKET_HPP

#include "ASocket.hpp"

/// For clients, Maybe usefull for cgi ?
class BindSocket: public ASocket
{
public:
	BindSocket(int family, int port, u_long interface, int service, int protocol);
	virtual ~BindSocket();

	virtual int connectToNetwork(int, sockaddr_in);

private:	
	BindSocket(const BindSocket &);
	BindSocket & operator=(const BindSocket &);
};

#endif /* CONNECTSOCKET_HPP */

BindSocket::BindSocket(int family = AF_INET, int port = 0, u_long interface, int service,
	int protocol): ASocket(family, port, interface, service, protocol)
{
	setConnection(
		connectToNetwork(getSocket(), getAddressIn())
	);		
}

int BindSocket::connectToNetwork(int sock, sockaddr_in address)
{
	return connect(sock, reinterpret_cast<sockaddr*>(&address), sizeof(address));
}

BindSocket::~BindSocket(){}
