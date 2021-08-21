/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/30 14:07:41 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/21 22:06:13 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "ServerHandler.hpp"

#include <iostream>
#include <iomanip>
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

void solidLine(int width, char ornament)
{
	std::cout << std::setw(width) << std::setfill(ornament);
	std::cout << ornament << std::endl;
}

void emptyLine(int width, char ornament)
{
	std::cout << ornament << std::setw(width - 2) << std::setfill(' ');
	std::cout << ' ' << ornament << std::endl;
}

void textLine(int width, char ornament, std::string text)
{
	int fillWidth = (width - 2 - text.size()) / 2;
	bool fill = (text.size() % 2 == 0 ? false : true);
	
	std::cout << ornament << std::setw(fillWidth) << std::setfill(' ');
	std::cout << ' ' << text;
	std::cout << std::setw(fillWidth) << std::setfill(' ');
	if (fill)
		std::cout << ' ' << ' ' << ornament << std::endl;
	else
		std::cout << ' ' << ornament << std::endl;
}

void prompt()
{
	int		width = 70;
	char	ornament = '*';

	std::cout << std::endl;
	solidLine(width, ornament);
	emptyLine(width, ornament);
	textLine(width, ornament, "Webserv - another 42 project");
	emptyLine(width, ornament);
	textLine(width, ornament, "v 1.0 | august 2020");
	textLine(width, ornament, "authors: hwinston, juligonz");
	emptyLine(width, ornament);
	solidLine(width, ornament);
	std::cout << std::endl;
}

int main(int ac, char** av)
{
	std::string input;
	parseArgs(ac, av);
	ServerConfig& config = ServerConfig::getInstance();
	server::ServerHandler sh(config.getPorts());
	sh.start();
	prompt();
	while (true)
		sh.update();
	return 0;
}