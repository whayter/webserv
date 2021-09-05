/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 19:22:37 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/30 22:24:47 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerHandler.hpp"
#include "HttpRequest.hpp"

#include "HttpResponse.hpp"

#include "utility.hpp"

#include <cerrno>
#include <ctime>
#include <iomanip>

#define BUF_SIZE 1048

/* --- Public functions ----------------------------------------------------- */

server::ServerHandler::ServerHandler()
{
	ServerConfig& config = ServerConfig::getInstance();
	std::vector<uint32_t> ports = config.getPorts();
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

bool server::ServerHandler::start(void)
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

void server::ServerHandler::run(void)
{
	_upToDateData = true;
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
			{
				_getRequest(i);
				if (_requests[i]->isComplete())
					_serveClient(i);
			}
		}
		else
			stop(-1);
	}
	if (!_upToDateData)
		_updateData();
}

void server::ServerHandler::stop(int status)
{
	for (int i = 0; i < _nfds; i++)
	{
		sckt::closeSocket(_fds[i].fd);
		delete _requests[i];
	}
	_servers.clear();
	if (status == -1)
	{
		_log(0, "An error has occurred.");
		_log (0, "Shutting down...");
		exit(EXIT_FAILURE);
	}
	else
		_log (0, "Shutting down...");
}

/* --- Private functions ---------------------------------------------------- */

void server::ServerHandler::_connectClients(int serverSocket)
{
	while (_nfds < SOMAXCONN)
	{
		sckt::fd_type newFd;
		newFd = accept(serverSocket, NULL, NULL);
		if (newFd == INVALID_FD)
			break;
		_log(newFd, "Connection accepted.");
		_requests[_nfds] = new HttpRequest();
		_fds[_nfds].fd = newFd;
		_fds[_nfds].events = POLLIN;
		_nfds++;
	}
}

void server::ServerHandler::_disconnectClient(int index)
{
	_log(_fds[index].fd, "Connection closed.");
	sckt::closeSocket(_fds[index].fd);
	delete _requests[index];
	_requests[index] = NULL;
	_fds[index].fd = -1;
	_upToDateData = false;
}

void server::ServerHandler::_serveClient(int index)
{
	ServerConfig& config = ServerConfig::getInstance();
	Uri uri = _requests[index]->getUri();
	HttpResponse response(config.findServer(uri), *_requests[index]);
	response.setMandatory();
	_sendResponse(index, response.toString().c_str());
	_requests[index]->clear();
}

void server::ServerHandler::_getRequest(int index)
{

	// send nbytes to read

	char buffer[BUF_SIZE] = {0};
	int nbytes = recv(_fds[index].fd, buffer, BUF_SIZE - 1, 0);
	if (nbytes < 0)
		stop(-1);
	else if (nbytes > 0)
		_requests[index]->read(buffer, nbytes);
	if (_requests[index]->isComplete())
		_log(_fds[index].fd, "Request received.");
}

void server::ServerHandler::_sendResponse(int index, const char* response)
{
	if (send(_fds[index].fd, response, strlen(response), 0) == -1)
	{
		_log(_fds[index].fd, "Could not send the response.");
		stop(-1);
	}
	_log(_fds[index].fd, "Response sent.");
}

bool server::ServerHandler::_isServerSocket(int index)
{
	return index < _firstClientIndex;
}

void server::ServerHandler::_updateData(void)
{
	for (int i = 0; i < _nfds; i++)
	{
		if (_fds[i].fd == -1)
		{
			for (int j = i; j < _nfds; j++)
			{
				_fds[j] = _fds[j + 1];
				_requests[j] = _requests[j + 1];
			}
			_nfds--;
			i--;
		}
	}
	_upToDateData = true;
}

void server::ServerHandler::_log(int fd, std::string message)
{
	std::cout << std::setw(4) << std::setfill(' ') << ' ';
	std::cout << "[" << getDate() << "]";
	std::cout << std::setw(4) << std::setfill(' ') << ' ';
	if (fd)
	{
		std::cout << std::setw(2) << std::setfill('0');
		std::cout << fd;
		std::cout << std::setw(4) << std::setfill(' ') << ' ';
	}
	std::cout << message << std::endl;
}