/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/30 14:07:41 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/16 15:31:32 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"
#include <vector>

int main(int ac, char** av)
{
	// if (ac != 2)
	// {
	// 	std::cerr << "Error: ";
	// 	std::cerr << "a configuration file is expected as the only parameter.";
	// 	std::cerr << std::endl;
	// 	return -1;
	// }
	// if (config = parseConfigFile(av[1]))
	// {
	// 	ServerHandler sh(config);
	// 	sh.run();
	// }
	// else
	// {
	// 	std::cerr << "Error: ";
	// 	std::cerr << "something is wrong in the configuration file.";
	// 	std::cerr << std::endl;
	// 	return -1;
	// }
	// return 0;

	/* temporary */

	(void)ac;
	(void)av;
	
	Server s(8080);
	Server ss(8081);
	s.start();
	ss.start();
	std::cout << "Webserv | v 1.0 | authors: juligonz, hwinston" << std::endl;
	std::cout << std::endl;
	while (true)
	{
		s.update();
		ss.update();
	}
	s.stop();
	ss.stop();


	// std::vector<int> ports;
	// ports.push_back(8080);
	// // ports.push_back(8081);
	// // ports.push_back(8082);
	// // ports.push_back(8083);

	// ServerHandler sh(ports);
	// sh.run();
	return 0;

	/* end temporary */
}