/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 19:22:37 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/23 10:21:58 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerHandler.hpp"
#include "HttpRequest.hpp"

#include <cerrno>

#include <ctime>
#include <iomanip>

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

void server::ServerHandler::run()
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
				_serveClient(i);
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
		_cout(2, "An error has occurred. ");
		_cout (2, "Shutting down...");
		exit(EXIT_FAILURE);
	}
	else
		_cout (2, "Shutting down...");
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
		_cout(newFd, "Connection accepted.");
		_requests[_nfds] = new HttpRequest();
		_fds[_nfds].fd = newFd;
		_fds[_nfds].events = POLLIN;
		_nfds++;
	}
}

void server::ServerHandler::_disconnectClient(int index)
{
	_cout(_fds[index].fd, "Connection closed.");
	sckt::closeSocket(_fds[index].fd);
	delete _requests[index];
	_requests[index] = NULL;
	_fds[index].fd = -1;
	_upToDateData = false;
}

// void server::ServerHandler::_serveClient(int index)			// à uncomment
// {
// 	_disconnect = false;
// 	if (!_requests[index]->isComplete())
// 		_getRequest(index);
// 	else
// 		_processRequest(index);
// 	if (_responses[index]->isReady())
// 		_sendResponse(index);
// 	if (_disconnect)
// 		_disconnectClient(index);
// }

// void server::ServerHandler::_getRequest(int index)			// à uncomment
// {
// 	char buffer[BUF_SIZE] = {0};
// 	int nbytes = recv(_fds[index].fd, buffer, BUF_SIZE - 1, 0);
// 	if (nbytes == -1)
// 		stop(-1);
// 	else if (nbytes == 0)
// 	{
// 		_cout(index, "recv() error.");
// 		_disconnect = true;
// 	}
// 	_cout(index, "Request received.");
// 	std::stringstream streamRequest(buffer);
// 	_requests[index]->read(streamRequest);
// }

void server::ServerHandler::_serveClient(int index)				// à supprimer
{
	_disconnect = false;
	if (!_getRequest(index))
		_disconnect = true;
	else
		_sendResponse(index);
	if (_disconnect == true)			// a mettre dans le else ?
		_disconnectClient(index);
}

bool server::ServerHandler::_getRequest(int index)				// à supprimer
{
	char buffer[BUF_SIZE] = {0};
	int nbytes = recv(_fds[index].fd, buffer, BUF_SIZE - 1, 0);
	if (nbytes == -1)
		stop(-1);
	else if (nbytes == 0)
	{
		_cout(_fds[index].fd, "recv() error.");
		return false;
	}
	_cout(_fds[index].fd, "Request received.");
	std::stringstream streamRequest(buffer);
	_requests[index]->read(streamRequest);
	return true;
}

void server::ServerHandler::_processRequest(int index)
{
	_cout(index, "Processing request.");
	// build response here
}

void server::ServerHandler::_sendResponse(int index)
{
	if (send(_fds[index].fd, HELLO, strlen(HELLO), 0) == -1)
	{
		_cout(_fds[index].fd, "send() error.");
		_disconnect = true;
	}
	_cout(_fds[index].fd, "Response sent.");
}

bool server::ServerHandler::_isServerSocket(int index)
{
	return index < _firstClientIndex;
}

void server::ServerHandler::_updateData()
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

void server::ServerHandler::_cout(int index, std::string message)
{
	time_t now = time(0);
	tm *ltm = localtime(&now);
	std::cout << std::setw(3) << std::setfill(' ') << ' ';
	std::cout << "[";
	std::cout << std::setw(2) << std::setfill('0');
	std::cout << ltm->tm_hour << ":";
	std::cout << std::setw(2) << std::setfill('0');
	std::cout << ltm->tm_min << ":";
	std::cout << std::setw(2) << std::setfill('0');
	std::cout << ltm->tm_sec << "]";

	std::cout << std::setw(3) << std::setfill(' ') << ' ';
	std::cout << std::setw(2) << std::setfill('0');
	std::cout << index;
	std::cout << " - ";
	std::cout << message << std::endl;
}