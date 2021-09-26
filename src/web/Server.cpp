/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 23:26:05 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/18 13:27:23 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "ServerConfig.hpp"
#include "utility.hpp"

#include "MessageBuilder.hpp"
#include "Response.hpp"

#include "parserMessage.hpp"

#include <cerrno>
#include <ctime>
#include <cstring>
#include <iomanip>
#include <unistd.h>

#define BUFFER_SIZE 1024

namespace web {

/* --- Public functions ----------------------------------------------------- */

Server::Server()
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

Server::~Server()
{
	stop(0);
}

bool Server::setup()
{
	std::vector<Device>::iterator device;
	for (device = _devices.begin(); device != _devices.end(); device++)
	{
		if (!device->getSocket().setFd(AF_INET, SOCK_STREAM))
			return false;
		device->getSocket().setAddr(AF_INET, INADDR_ANY, device->getPort());
		if (!device->setNonBlocking() || !device->setReusableAddr()
		|| !device->bindSocket() || !device->listenSocket())
			stop(-1);
		_fds[_nfds].fd = device->getSocket().getFd();
		_fds[_nfds].events = POLLIN;
		_nfds++;
	}
	_firstClientIndex = _nfds;
	return true;
}

void Server::routine()
{
	int pollStatus;
	if ((pollStatus = poll(_fds, _nfds, 0)) == -1)
	{
		if (ft::make_error_code().value() == ft::errc::interrupted)
			return;
		else
			stop(-1);
	}
	for (int i = 0; i < _nfds; i++)
	{
		if (_fds[i].revents == 0)
			continue;
		else if (_fds[i].revents & POLLHUP)
			_disconnectDevice(i);
		else if (_fds[i].revents & POLLIN)
		{
			if (_isServerIndex(i))
				_connectClients(i);
			else
			{
				_getRequests(i);
				_buildRequests(i);
				_buildResponses(i);
				_sendResponses(i);
			}
		}
		else
			stop(-1);
	}
}

void Server::stop(int status)
{
	for (int i = _nfds - 1; i >= 0; i--)
		_disconnectDevice(i);
	_devices.clear();
	if (status == -1)
	{
		_log(-1, "An error has occurred. Shutting down...");
		exit(EXIT_FAILURE);
	}
	else
		_log(-1, "Shutting down...");
}

/* --- Private functions ---------------------------------------------------- */

void Server::_connectClients(int serverIndex)
{
	while (true)
	{
		if (_nfds == SOMAXCONN)
			_disconnectDevice(_firstClientIndex);
		Device newClient;
		newClient.getSocket().setFd(accept(_devices[serverIndex].getSocket().getFd(), NULL, NULL));
		if (newClient.getSocket().getFd() == INVALID_FD)
			break;
		newClient.setPort(_devices[serverIndex].getPort());
		_devices.push_back(newClient);
		_log(_nfds, "Connection accepted.");
		_fds[_nfds].fd = newClient.getSocket().getFd();
		_fds[_nfds].events = POLLIN;
		_nfds++;
	}
}

void Server::_disconnectDevice(int deviceIndex)
{
	_log(deviceIndex, "Connection closed.");
	_devices[deviceIndex].closeSocket();
	_devices.erase(_devices.begin() + deviceIndex);
	for (int i = deviceIndex; i < _nfds; i++)
		_fds[i] = _fds[i + 1];
	_nfds--;
}

void Server::_getRequests(int deviceIndex)
{
	unsigned char buffer[BUFFER_SIZE] = {0};
	int nbytes = recv(_fds[deviceIndex].fd, buffer, BUFFER_SIZE, 0);
	if (nbytes < 0)
		stop(-1);
	else if (nbytes > 0)
	{
		for (int i = 0; i < nbytes; i++)
			_devices[deviceIndex].getInputBuffer().push_back(buffer[i]);
	}
}

void Server::_buildRequests(int deviceIndex)
{
	http::Request request;
	http::Status errorCode;

	while (http::parseRequest(request, errorCode, _devices[deviceIndex].getInputBuffer()))
	{
		_devices[deviceIndex].getRequestsQueue().push(request);
		_log(deviceIndex, "Request received.");
	}
}

void Server::_buildResponses(int deviceIndex)
{
	http::MessageBuilder builder;
	std::queue<http::Request>& requests = _devices[deviceIndex].getRequestsQueue();
	while (!requests.empty())
	{
		http::Response response = builder.buildResponse(requests.front());
		requests.pop();
		_devices[deviceIndex].getResponsesQueue().push(response);
	}
}

void Server::_sendResponses(int deviceIndex)
{
	http::MessageBuilder builder;
	std::queue<http::Response>& responses = _devices[deviceIndex].getResponsesQueue();
	std::vector<unsigned char> outputBuffer = _devices[deviceIndex].getOutputBuffer();

	while (!responses.empty())
	{
		http::Response response = responses.front();
		std::string stringResponse = builder.stringifyMessage(response);
		outputBuffer.insert(outputBuffer.end(), stringResponse.begin(), stringResponse.end());
		responses.pop();
	}
	if (!outputBuffer.empty())
	{
		int nbytes = send(_fds[deviceIndex].fd, &outputBuffer[0], outputBuffer.size(), 0);
		if (nbytes == -1)
		{
			_log(deviceIndex, "Could not send the response.");
			stop(-1);
		}
		outputBuffer.erase(outputBuffer.begin(), outputBuffer.begin() + nbytes);
		_log(deviceIndex, "Response sent.");
	}
}

bool Server::_isServerIndex(int deviceIndex)
{
	return deviceIndex < _firstClientIndex;
}

void Server::_log(int deviceIndex, std::string description)
{
	std::cout << "[" << ft::getDate() << "]";
	std::cout << std::setw(4) << std::setfill(' ') << ' ';
	if (deviceIndex != -1)
	{
		std::cout << std::setw(2) << std::setfill('0');
		std::cout << _devices[deviceIndex].getSocket().getFd();
		std::cout << std::setw(4) << std::setfill(' ') << ' ';
	}
	std::cout << description << std::endl;
}

}; /* namespace web */