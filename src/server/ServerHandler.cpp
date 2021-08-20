/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 19:22:37 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/20 16:41:50 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerHandler.hpp"
#include "HttpRequest.hpp"

#include <cerrno>
#include <vector>

#define BUF_SIZE 4096
#define HELLO "HTTP/1.1 200 OK\nContent-Type: text/html;charset=UTF-8\nContent-Length: 133\n\n<style>html{background-color:black;color:white;text-align:center}</style><html><body><h1>Webserv</h1><h2>Yeah man!</h2></body></html>"

/* --- Public functions ----------------------------------------------------- */

server::ServerHandler::ServerHandler(std::vector<uint32_t> ports)
{
	std::vector<uint32_t>::iterator port;

	for (port = ports.begin(); port != ports.end(); port++)
	{
		server::Server newServer;
		newServer.port = *port;
		_servers.push_back(newServer);
	}
	memset(_fds, 0, sizeof(_fds));
	_nfds = 0;
}

server::ServerHandler::~ServerHandler()
{
	stop(0);
}

bool server::ServerHandler::start()
{
	std::vector<Server>::iterator server;
	for (server = _servers.begin(); server != _servers.end(); server++)
	{
		if (!server->socket.setFd(AF_INET, SOCK_STREAM))
			return false;
		server->socket.setAddr(AF_INET, INADDR_ANY, server->port);
		if (!sckt::setNonBlocking(server->socket.getFd())
		|| !sckt::setReusableAddr(server->socket.getFd())
		|| !sckt::bindSocket(server->socket.getFd(), server->socket.getAddr())
		|| !sckt::listenSocket(server->socket.getFd()))
			stop(-1);
		_fds[_nfds].fd = server->socket.getFd();
		_fds[_nfds].events = POLLIN;
		server->index = _nfds;
		_nfds++;		
	}
	_firstClientIndex = _nfds;
	return true;
}

void server::ServerHandler::update()
{
	_upToDateFds = true;
	int pollStatus;
	if ((pollStatus = poll(_fds, _nfds, 0)) == -1)
		stop(-1);
	for (int i = 0; i < _nfds; i++)
	{
		if (_fds[i].revents == 0)
			continue;
		else if (_fds[i].revents & POLLHUP)
			_disconnectClient(i);
		else if (_fds[i].revents & POLLIN)
		{
			if (_isServerSocket(i))
				_connectClients(_fds[i].fd);
			else
				_serveClient(i);
		}
		else
			stop(-1);
	}
	if (!_upToDateFds)
		_updateFds();
}

void server::ServerHandler::stop(int status)
{
	for (int i = 0; i < _nfds; i++)
		sckt::closeSocket(_fds[i].fd);
	_servers.clear();
	if (status == -1)
	{
		std::cout << "An error has occurred. " << std::endl;
		std::cout << "The server is shutting down..." << std::endl;
		exit(EXIT_FAILURE);
	}
}

/* --- Private functions ---------------------------------------------------- */

void server::ServerHandler::_connectClients(int serverSocket)
{
	while (true)
	{
		sckt::fd_type newFd;
		newFd = accept(serverSocket, NULL, NULL);
		if (newFd == INVALID_FD)
			break;
		std::cout << "[" << _nfds << "]	";
		std::cout << "Connection." << std::endl;
		_fds[_nfds].fd = newFd;
		_fds[_nfds].events = POLLIN;
		_nfds++;
	}
}

void server::ServerHandler::_disconnectClient(int index)
{
	std::cout << "[" << index << "]	";
	std::cout << "Disconnection." << std::endl;
	sckt::closeSocket(_fds[index].fd);
	_fds[index].fd = -1;
	_upToDateFds = false;
}

void server::ServerHandler::_serveClient(int index)
{
	bool disconnect = false;
	std::cout << "[" << index << "]	";
	std::cout << "Getting request: ";
	if (!_getRequest(_fds[index].fd))
	{
		std::cout << "failure." << std::endl;
		disconnect = true;
	}
	else
	{
		if (send(_fds[index].fd, HELLO, strlen(HELLO), 0) <= 0)
			disconnect = true; 
		if (disconnect == true)
			_disconnectClient(index);
	}
}

bool server::ServerHandler::_getRequest(int fd)
{
	char buffer[BUF_SIZE] = {0};
	int nbytes = recv(fd, buffer, BUF_SIZE - 1, 0);
	if (nbytes == -1)
		stop(-1);
	else if (nbytes == 0)
		return false;

	std::cout << "success (" << nbytes;
	std::cout << " bytes received)." << std::endl;

	// HttpRequest request;
	// std::stringstream streamRequest(buffer);
	// request = HttpRequest::create(streamRequest);
	// std::cout << request.getUri().toString() << std::endl;
	return true;
}

bool server::ServerHandler::_isServerSocket(int index)
{
	return index < _firstClientIndex;
}

void server::ServerHandler::_updateFds()
{
	for (int i = 0; i < _nfds; i++)
	{
		if (_fds[i].fd == -1)
		{
			for (int j = i; j < _nfds; j++)
			{
				_fds[j].fd = _fds[j + 1].fd;
				_fds[j].events = _fds[j + 1].events;
				_fds[j].revents = _fds[j + 1].revents;
			}
				//_fds[j] = _fds[j + 1];
			_nfds--;
			i--;
		}
	}
	_upToDateFds = true;
}
