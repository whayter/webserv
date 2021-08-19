/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 19:22:37 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/16 15:33:02 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"
#include "http/HttpRequest.hpp"

#include <cerrno>
#include <vector>

/* --- Public functions ----------------------------------------------------- */

Server::Server(int port): _port(port) {}

Server::~Server()
{
	stop();
}

bool Server::start()
{
	if (!_socket.setFd(AF_INET, SOCK_STREAM))
		return false;
	_socket.setAddr(AF_INET, INADDR_ANY, _port);
	if (!sckt::setNonBlocking(_socket.getFd())
	|| !sckt::setReusableAddr(_socket.getFd())
	|| !sckt::bindSocket(_socket.getFd(), _socket.getAddr())
	|| !sckt::listenSocket(_socket.getFd()))
	{
		stop();
		return false;
	}
	memset(_fds, 0, sizeof(_fds));
	_fds[0].fd = _socket.getFd();
	_fds[0].events = POLLIN;
	_nfds = 1;
	return true;
}

void Server::update()
{
	_upToDateFds = true;
	int pollStatus;
	if ((pollStatus = poll(_fds, _nfds, 0)) == -1)
		stop();
	for (int i = 0, s = _nfds; i < s; i++)
	{
		if (_fds[i].revents == 0)
			continue;
		if (_fds[i].revents != POLLIN)
			stop();
		if (_fds[i].fd == _socket.getFd())
			while (_connectClient())
				continue;
		else
			_manageClient(i);
	}
	if (!_upToDateFds)
		_updateFds();
}

void Server::stop()
{
	for (int i = 0; i < _nfds; i++)
		sckt::closeSocket(_fds[i].fd);
}

/* --- Private functions ---------------------------------------------------- */

void Server::_updateFds()
{
	for (int i = 0; i < _nfds; i++)
	{
		if (_fds[i].fd == -1)
		{
			for (int j = i; j < _nfds; j++)
				_fds[j].fd = _fds[j + 1].fd;
			_nfds--;
			i--;
		}
	}
	_upToDateFds = true;
}

void Server::_manageClient(int index)
{
	bool disconnect = false;
	if (!_getRequest(_fds[index].fd))
		disconnect = true;
	else
	{
		const char* hello = "HTTP/1.1 200 OK\nContent-Type: text/html;charset=UTF-8\nContent-Length: 133\n\n<style>html{background-color:black;color:white;text-align:center}</style><html><body><h1>Webserv</h1><h2>Yeah man!</h2></body></html>";
		if (send(_fds[index].fd, hello, strlen(hello), 0) <= 0)
			disconnect = true; 
		if (disconnect)
			_disconnectClient(index);
	}
}

bool Server::_getRequest(int fd)
{
	char buffer[32000] = {0};
	int nbytes = recv(fd, buffer, 32000 - 1, 0);
	if (nbytes <= 0 && nbytes != EWOULDBLOCK)
		return false;
	HttpRequest request;
	std::stringstream streamRequest(buffer);
	request = HttpRequest::create(streamRequest);
	std::cout << request.toString() << std::endl;		// tmp
	std::cout << std::endl << std::endl;				// tmp
	return true;
}

bool Server::_connectClient()
{
	sckt::fd_type fd;
	fd = accept(_socket.getFd(), NULL, NULL);
	if (fd == INVALID_FD)
		return false;
	_fds[_nfds].fd = fd;
	_fds[_nfds].events = POLLIN;
	_nfds++;

	std::cout << "New connection [" << fd << "]" << std::endl;
	return true;
}

void Server::_disconnectClient(int index)
{
	std::cout << "Disconnection [" << _fds[index].fd << "]" << std::endl;

	sckt::closeSocket(_fds[index].fd);
	_fds[index].fd = -1;
	_upToDateFds = true;
}
