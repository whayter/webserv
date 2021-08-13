/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 16:05:35 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/13 15:13:36 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/ServerHandler.hpp"

ServerHandler::ServerHandler(std::vector<int> ports)
{
	std::vector<int>::iterator port;
	for (port = ports.begin(); port != ports.end(); port++)
		_servers.push_back(Server(*port));
	std::vector<Server>::iterator server;
	for (server = _servers.begin(); server != _servers.end(); server++)
 		server->start();
}

ServerHandler::~ServerHandler()
{
	_servers.clear();
}

void ServerHandler::run()
{
	std::cout << "Webserv | v 1.0 | authors: juligonz, hwinston" << std::endl;
	std::cout << std::endl;
	
	std::vector<Server>::iterator server;
	while (true)
	{
		server = _servers.begin();
		for (; server != _servers.end(); server++)
			server->update();
	}
}
