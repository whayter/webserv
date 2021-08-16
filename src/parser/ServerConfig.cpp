/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 15:01:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/16 20:06:28 by juligonz         ###   ########.fr       */
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

void ServerConfig::_throw_SyntaxError(parser::config::Token t, const std::string &error_str)
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

void ServerConfig::_skipSemiColonNewLine(parser::config::ScannerConfig & scanner)
{
	pr::Token t;

	if ((t = scanner.getToken()).kind != pr::ScopedEnum::kSemiColon)
		_throw_SyntaxError(t, "Missing semi-colon.");
	if ((t = scanner.getToken()).kind != pr::ScopedEnum::kNewLine)
		_throw_SyntaxError(t, "Missing new line after semi-colon.");
}


void ServerConfig::_parse(std::istream & in)
{
	pr::ScannerConfig scanner(in);

	pr::Token t;
	while ((t = scanner.getToken()).kind != pr::ScopedEnum::kEndOfInput)
	{
		std::cout<< t << std::endl;
		if (t.kind== pr::ScopedEnum::kString && t.value == "server")
			_servers.push_back(_parseServer(scanner));
		else
			_throw_SyntaxError(t,
				"Unknown identifier \"" + t.value + "\" at root context");

		std::cout << t ;
	}

	
}

ServerBlock ServerConfig::_parseServer(pr::ScannerConfig & scanner)
{
	ServerBlock result;
	pr::Token t;

	if ((t = scanner.getToken(true)).kind != pr::ScopedEnum::kLeftBrace)
		_throw_SyntaxError(t, "Missing open brace at server block.");
	while ((t = scanner.getToken(true)).kind != pr::ScopedEnum::kEndOfInput
	 && t.kind != pr::ScopedEnum::kRightBrace)
	{
		std::cout << t << std::endl;
		switch (t.kind)
		{
			case pr::ScopedEnum::kString :
				if (t.value == "listen")
					result.listens.push_back(_parseListen(scanner));
				else if (t.value == "root")
					result.root = _parseRoot(scanner);
				else if (t.value == "index")
					_parseIndex(scanner);
				else if (t.value == "server_name")
					_parseServerName(scanner);
				else if (t.value == "error_page")
					_parseErrorPage(scanner);
				else if (t.value == "location")
					_parseLocation(scanner);
				else
					_throw_SyntaxError(t,
						"Unknown identifier \"" + t.value + "\" in context 'server'");
				break;
			case pr::ScopedEnum::kNewLine :
					continue;
				break;
			case pr::ScopedEnum::kSemiColon :
				_throw_SyntaxError(t, "Unexpected semi-colon.");
				break;
			default:
				_throw_SyntaxError(t, "Ho shit, not expected:  " + pr::tokenToString(t));
				break;
		}
	}
	return result;
}

Host ServerConfig::_parseListen(parser::config::ScannerConfig & scanner)
{
	Host result;
	pr::Token t;

	if ((t = scanner.getToken()).kind != pr::ScopedEnum::kString)
		_throw_SyntaxError(t, "Bad character in \"listen\" context.");
	result = _parseHost(t.value);
	_skipSemiColonNewLine(scanner);
	return result;
}

std::string ServerConfig::_parseRoot(parser::config::ScannerConfig & scanner)
{
	std::string result;
	pr::Token t;

	if ((t = scanner.getToken()).kind != pr::ScopedEnum::kString)
		_throw_SyntaxError(t, "Bad token in root context");
	_skipSemiColonNewLine(scanner);
	std::cout << "	>root> " <<  t << std::endl;
	return result;
}

std::string ServerConfig::_parseIndex(parser::config::ScannerConfig & scanner)
{
	std::string result;
	(void)scanner;
	
	return result;
}

void ServerConfig::_parseServerName(parser::config::ScannerConfig & scanner)
{
	(void)scanner;
	return;
	
}

void ServerConfig::_parseErrorPage(parser::config::ScannerConfig & scanner)
{
	(void)scanner;
	return;
	
}

void ServerConfig::_parseLocation(parser::config::ScannerConfig & scanner)
{
	(void)scanner;
	
}

Host ServerConfig::_parseHost(const std::string& host)
{
	Host result;
	u_short port = 0;

    std::string::const_iterator it = host.begin();
    std::string::const_iterator end = host.end();

    while(it != end && *it != ':')
        result.host += *it++;
    
    if (it != end && *it == ':')
    {
        it++;
        while (it != end && isdigit(*it))
        {
            port = port * 10 + *it - '0';
            it++;
        }
    }
    lowerStringInPlace(result.host);
	result.port = port;
	return result;
}
