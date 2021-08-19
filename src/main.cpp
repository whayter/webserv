/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/30 14:07:41 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/19 18:02:06 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "Server.hpp"

#include <vector>

void parseArgs(int ac, char **av)
{
	std::string path = "config/webserv.conf";

	if (ac == 2)
		path = av[1];
	try
	{
		ServerConfig::getInstance(path);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what();
		exit(1);
	}
}

int main(int ac, char** av)
{
	parseArgs(ac, av);
	ServerConfig& config = ServerConfig::getInstance();
	std::vector<uint32_t> serverPorts = config.getPorts();
	return 0;
}