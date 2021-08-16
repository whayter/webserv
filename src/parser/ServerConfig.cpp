/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 15:01:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/16 16:03:58 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/ServerConfig.hpp"
#include "parser/config/ScannerConfig.hpp"

#include <fstream>
#include <exception>

ServerConfig* ServerConfig::_singleton = nullptr;

namespace pr = parser::config;

ServerConfig::ServerConfig(const std::string& filepath)
	: _configFilePath(filepath)
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
	pr::ScannerConfig scanner(in);

	pr::Token t;
	while ((t = scanner.getToken()).kind != pr::ScopedEnum::kEndOfInput)
	{
		std::cout<< t << std::endl;
		if (t.kind== pr::ScopedEnum::kString && t.value == "server")
			_parseServer(scanner);

	}

	
	std::cout << t ;
}

void ServerConfig::_parseServer(pr::ScannerConfig & scanner)
{
	pr::Token t;

	if (scanner.getToken(true).kind != pr::ScopedEnum::kLeftBrace)
		_thow_SyntaxError(t, "Missing ");
	if (scanner.getToken(true).kind != pr::ScopedEnum::kLeftBrace)
		_thow_SyntaxError(t, "Missing ");
	while ((t = scanner.getToken()).kind != pr::ScopedEnum::kEndOfInput)
	{
		// pr::Token t = scanner.getToken();
		std::cout << "       >>      " <<   t << std::endl;
	}

}


ServerConfig* ServerConfig::getInstance(std::string filepath){
	if (_singleton == NULL)
		_singleton = new ServerConfig(filepath);
	return _singleton;
}


void ServerConfig::_thow_SyntaxError(parser::config::Token t, const std::string &error_str)
{
	std::string error;

	error += _configFilePath + ':';
	error += t.line;
	error += ':';
	error += t.column;
	error += ": error: ";
	error += error_str;
	throw ServerConfig::SyntaxError(error);
}