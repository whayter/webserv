/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 11:37:05 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/14 17:49:44 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"
#include "http/HttpRequest.hpp"
#include "http/ParserHttpRequest.hpp"

#include <cerrno>

Server::Server(int port): _port(port) {}

Server::Server(const Server& s)
{
	*this = s;
}

Server& Server::operator=(const Server& s)
{
	_port = s._port;
	_socket = s._socket;
	_clients = s._clients;
	_pfd = s._pfd;
	return *this;
}
		
Server::~Server()
{
	stop();
}

bool Server::start()
{
	if (!_socket.setFd(AF_INET, SOCK_STREAM))
		return false;
	if (!sckt::setNonBlocking(_socket.getFd())
	|| !sckt::setReusableAddr(_socket.getFd()))
	{
		stop();
		return false;
	}
	_socket.setAddr(AF_INET, INADDR_ANY, _port);
	_pfd.fd = _socket.getFd();
	_pfd.events = POLLIN;
	if (!sckt::bindSocket(_socket.getFd(), _socket.getAddr()))
	{
		stop();
		return false;
	}
	if (!sckt::listenSocket(_socket.getFd()))
	//if (!sckt::bindSocket(_socket.getFd(), _socket.getAddr()))
	{
		stop();
		return false;
	}
	return true;
}

void Server::update()
{
	int pollStatus;
	if ((pollStatus = poll(&_pfd, 1, 0)) == -1)
		stop();
	else if (pollStatus > 0)
		connectClient();
	clients_type::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		struct pollfd clientPfd = it->getPfd();
		if ((pollStatus = poll(&clientPfd, 1, 0)) > 0)
			manageClient(it);
	}
}

void Server::stop()
{
	clients_type::iterator it = _clients.begin();
	for (it = _clients.begin(); it != _clients.end(); it++)
		clnt::disconnect(*it);
	_clients.clear();
	sckt::closeSocket(_socket.getFd());
}

void Server::connectClient()
{
	sckt::fd_type fd;
	sckt::addr_type addr;
	sckt::addrLen_type len = sizeof(addr);
	fd = accept(_socket.getFd(), (struct sockaddr *)&addr, &len);
	if (fd == INVALID_FD)
		return ;
	clnt::Client newClient;
	newClient.setFd(fd);
	newClient.setAddr(addr);
	newClient.setPfd();
	_clients.push_back(newClient);
}

void Server::manageClient(clients_type::iterator client)
{
	bool disconnect = false;

	if (!getReq(client->getFd()))
		disconnect = true;

	// std::string stringRequest;
	// if (!getRequest(client->getFd(), &stringRequest))
	// 	disconnect = true;

	else
	{
		// parse request here
		// send resonse
		// in the meantime, return a static page

		//std::cout << stringRequest << std::endl;
		const char* hello = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\nContent-Length: 133\n\n<style>html{background-color:black;color:white;text-align:center}</style><html><body><h1>Webserv</h1><h2>Yeah man!</h2></body></html>";
		if (send(client->getFd(), hello, strlen(hello), 0) <= 0)
			disconnect = true; 
		if (disconnect)		// sortir du else ?
			disconnectClient(client);
	}
}

bool Server::getReq(sckt::fd_type fd)
{
	char buffer[64000] = {0};
	int nbytes = recv(fd, buffer, 64000 - 1, 0);
	if (nbytes <= 0 && nbytes != EWOULDBLOCK)
		return false;

	HttpRequest request;
	std::stringstream streamRequest(buffer);
	request = parser::http::ParserHttpRequest::create(streamRequest);

	std::cout << "REQUEST:" << std::endl;
	std::cout << request.toString() << std::endl;

	return true;
}

bool Server::getRequest(sckt::fd_type fd, std::string* stringRequest)
{
	char buffer[64000] = {0};
	int nbytes = 0;

	while (nbytes == 0)
	{
		nbytes = recv(fd, buffer, 64000 - 1, 0);
		if (nbytes <= 0 && nbytes != EWOULDBLOCK)
			return false;
		*stringRequest += buffer;
	}
	return true;

	// char buffer[10000] = {0};
	// int ret = recv(fd, buffer, 10000 - 1, 0);
	// if (ret <= 0 && ret != EWOULDBLOCK)
	// 	return false;
	// *stringRequest = buffer;
	// return true;
}

void Server::disconnectClient(clients_type::iterator client)
{
	clnt::disconnect(*client);
	_clients.erase(client);
}
