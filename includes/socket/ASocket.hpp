/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 11:38:24 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/04 00:14:02 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>

class ASocket
{
public:
	ASocket(int family, int port, u_long interface, int service, int protocol);
	virtual ~ASocket();

	void				testConnection(int);

	inline sockaddr_in	getAddressIn()	{ return _address; }
	inline int			getSocket()		{ return _socket; }
	inline int			getConnection() { return _connection; }

	void				setConnection(int);

	virtual int			connectToNetwork(int sock, sockaddr_in address) = 0;

private:
	ASocket(const ASocket &);
	ASocket & operator=(const ASocket &);

	sockaddr_in	_address;
	int			_socket;
	int			_connection;
};


ASocket::ASocket(int family, int port, u_long interface, int service, int protocol)
{
	_address.sin_family = family;						//By default AF_INET (ipv4)
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = htonl(interface);		// Specify which sources are accepted
	_socket = socket(family, service, protocol);
	testConnection(_socket);
}

ASocket::~ASocket(){}

void ASocket::testConnection(int toTest)// Basically return a file descriptor// Basically return a file descriptor
{
	if (toTest >= 0)
		return;
	perror("Failed to connect:");
	exit(1);
}

void	ASocket::setConnection(int connection)
{
	_connection = connection;	
}


#endif /* ASOCKET_HPP */