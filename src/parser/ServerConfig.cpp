/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 15:01:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/16 17:04:32 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/ServerConfig.hpp"
#include "parser/config/ScannerConfig.hpp"
#include "utility.hpp"

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

ServerConfig* ServerConfig::getInstance(std::string filepath){
	if (_singleton == NULL)
		_singleton = new ServerConfig(filepath);
	return _singleton;
}

void ServerConfig::_thow_SyntaxError(parser::config::Token t, const std::string &error_str)
{
	std::string error;

	error += _configFilePath + ':';
	error += intToString(t.line);
	error += ':';
	error += intToString(t.column);
	error += ": error: ";
	error += error_str;
	throw ServerConfig::SyntaxError(error);
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
	pr::Token previous_t;
	pr::Token t;

	if ((t = scanner.getToken(true)).kind != pr::ScopedEnum::kLeftBrace)
		_thow_SyntaxError(t, "Missing open brace at server block.");
	previous_t = t;
	while ((t = scanner.getToken(true)).kind != pr::ScopedEnum::kEndOfInput
	 && t.kind != pr::ScopedEnum::kRightBrace)
	{
		switch (t.kind)
		{
			case pr::ScopedEnum::kString :
				if (t.value == "listen")
					_parseListen(scanner);
				else if (t.value == "root")
					_parseRoot(scanner);
				else if (t.value == "index")
					_parseIndex(scanner);
				else if (t.value == "server_name")
					_parseServerName(scanner);
				else if (t.value == "error_page")
					_parseErrorPage(scanner);
				else if (t.value == "location")
					_parseLocation(scanner);
				else
					_thow_SyntaxError(t,
						"Unknown identifier \"" + t.value + "\" in context 'server'");
				break;
			case pr::ScopedEnum::kNewLine :
				if ((t = scanner.getToken(true)).kind != pr::ScopedEnum::kLeftBrace)
				_thow_SyntaxError(t, "New Line");
				break;
			case pr::ScopedEnum::kColon :
					_thow_SyntaxError(t, "");
				break;
			
			default:
				_thow_SyntaxError(t, "Ho shit 2");
				break;
		}
		std::cout << "       >> " <<   t << std::endl;
	}

}


void ServerConfig::_parseListen(parser::config::ScannerConfig &)
{
	
}

void ServerConfig::_parseRoot(parser::config::ScannerConfig &)
{
	
}

void ServerConfig::_parseIndex(parser::config::ScannerConfig &)
{
	
}

void ServerConfig::_parseServerName(parser::config::ScannerConfig &)
{
	
}

void ServerConfig::_parseErrorPage(parser::config::ScannerConfig &)
{
	
}

void ServerConfig::_parseLocation(parser::config::ScannerConfig &)
{
	
}
