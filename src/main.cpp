/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/30 14:07:41 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/19 17:00:25 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "Server.hpp"

#include <vector>

void getConfigFile(char* path)
{
	try
	{
		ServerConfig::getInstance(path);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what();
	}
}

int main(int ac, char** av)
{
	if (ac < 2)
		getConfigFile("config/webserv.conf");
	else if (ac == 2)
		getConfigFile(av[1]);
	else
		return -1;
	ServerConfig& config = ServerConfig::getInstance();
	std::vector<uint32_t> serverPorts = config.getPorts();
	return 0;
}