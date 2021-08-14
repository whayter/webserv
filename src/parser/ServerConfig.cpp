/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 15:01:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/13 17:02:04 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/ServerConfig.hpp"
#include "parser/ScannerConfig.hpp"

#include <fstream>
#include <exception>

ServerConfig* ServerConfig::_singleton = nullptr;

namespace pr = parser;

ServerConfig::ServerConfig(const std::string& filepath)
	: _filepath(filepath)
{
	std::ifstream file;

	file.open(filepath, std::ifstream::in);
	if (!file.is_open())
		throw std::invalid_argument("Can't open file");
	_parse(file);
	file.close();
}

void ServerConfig::_parse(std::istream & in)
{
	pr::ScannerConfig _scanner(in);

	pr::Token t = _scanner.getToken();
	std::cout << t ;
}


ServerConfig* ServerConfig::getInstance(std::string filepath){
	if (_singleton == NULL)
		_singleton = new ServerConfig(filepath);
	return _singleton;
}
