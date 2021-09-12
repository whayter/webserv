/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 23:26:05 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/11 15:26:25 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "ServerConfig.hpp"
#include "utility.hpp"
#include "HttpResponse.hpp"

#include <cerrno>
#include <ctime>
#include <iomanip>
#include <unistd.h>

#define BUFFER_SIZE 1048

/* --- Public functions ----------------------------------------------------- */

web::Server::Server()
{
    ServerConfig& config = ServerConfig::getInstance();
	std::vector<uint32_t> ports = config.getPorts();
	std::vector<uint32_t>::iterator port;
	for (port = ports.begin(); port != ports.end(); port++)
	{
		Device newServer;
		newServer.setPort(*port);
		_devices.push_back(newServer);
	}
	memset(_fds, 0, sizeof(_fds));
	_nfds = 0;
}

web::Server::~Server()
{
	stop(0);
}

bool web::Server::setup()
{
	std::vector<Device>::iterator device;
	for (device = _devices.begin(); device != _devices.end(); device++)
	{
		if (!device->getSocket().setFd(AF_INET, SOCK_STREAM))
			return false;
		device->getSocket().setAddr(AF_INET, INADDR_ANY, device->getPort());
		if (!setNonBlocking(device->getSocket().getFd())
		|| !setReusableAddr(device->getSocket().getFd())
		|| !bindSocket(device->getSocket().getFd(), device->getSocket().getAddr())
		|| !listenSocket(device->getSocket().getFd()))
			stop(-1);
		_fds[_nfds].fd = device->getSocket().getFd();
		_fds[_nfds].events = POLLIN;
		_nfds++;
	}
	_firstClientIndex = _nfds;
	return true;
}

void web::Server::routine()
{
	int pollStatus;
	if ((pollStatus = poll(_fds, _nfds, 0)) == -1)
		stop(-1);
	for (int i = 0; i < _nfds; i++)
	{
		if (_fds[i].revents == 0)
			continue;
		else if (_fds[i].revents & POLLHUP)
			_disconnectDevice(i);
		else if (_fds[i].revents & POLLIN)
		{
			if (_isServerIndex(i))
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
}

void web::Server::stop(int status)
{
	for (int i = 0; i < _nfds; i++)
		_disconnectDevice(i);
	_devices.clear();
	if (status == -1)
	{
		_log(0, "An error has occurred. Shutting down...");
		exit(EXIT_FAILURE);
	}
	else
		_log (0, "Shutting down...");
}

/* --- Private functions ---------------------------------------------------- */

void web::Server::_connectClients(int serverFd)
{
	while (true)
	{
		if (_nfds == SOMAXCONN)
			_disconnectDevice(_firstClientIndex);
		Device newClient;
		newClient.getSocket().setFd(accept(serverFd, NULL, NULL));
		if (newClient.getSocket().getFd() == INVALID_FD)
			break;
		_devices.push_back(newClient);
		_log(newClient.getSocket().getFd(), "Connection accepted.");
		_requests[_nfds] = new HttpRequest();
		_fds[_nfds].fd = newClient.getSocket().getFd();
		_fds[_nfds].events = POLLIN;
		_nfds++;
	}
}

void web::Server::_disconnectDevice(int deviceIndex)
{
	_log(_fds[deviceIndex].fd, "Connection closed.");
	closeSocket(_fds[deviceIndex].fd);
	delete _requests[deviceIndex];
	_requests[deviceIndex] = NULL;
	_devices.erase(_devices.begin() + deviceIndex);
	for (int i = deviceIndex; i < _nfds; i++)
	{
		_fds[i] = _fds[i + 1];
		_requests[i] = _requests[i + 1];
	}
	_nfds--;
}

void web::Server::_serveClient(int index)
{
	ServerConfig& config = ServerConfig::getInstance();
	Uri uri = _requests[index]->getUri();
	HttpResponse response(config.findServer(uri), *_requests[index]);
	response.build();
	_sendResponse(index, response.toString().c_str());
	_requests[index]->clear();
}

void web::Server::_getRequest(int index)
{
	char buffer[BUFFER_SIZE] = {0};
	int nbytes = recv(_fds[index].fd, buffer, BUFFER_SIZE - 1, 0);
	if (nbytes < 0)
		stop(-1);
	else if (nbytes > 0)
		_requests[index]->read(buffer, nbytes);
	if (_requests[index]->isComplete())
		_log(_fds[index].fd, "Request received.");
}

void web::Server::_sendResponse(int index, const char* response)
{
	if (send(_fds[index].fd, response, strlen(response), 0) == -1)
	{
		_log(_fds[index].fd, "Could not send the response.");
		stop(-1);
	}
	_log(_fds[index].fd, "Response sent.");
}

bool web::Server::_isServerIndex(int index)
{
	return index < _firstClientIndex;
}

void web::Server::_log(int fd, std::string description)
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
	std::cout << description << std::endl;
}