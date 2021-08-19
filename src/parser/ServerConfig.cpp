/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 15:01:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/19 19:10:06 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/ServerConfig.hpp"
#include "parser/config/ScannerConfig.hpp"
#include "utility.hpp"

#include <fstream>
#include <exception>
#include <cstdlib>

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

ServerConfig& ServerConfig::getInstance(std::string filepath){
	if (_singleton == NULL)
		_singleton = new ServerConfig(filepath);
	return *_singleton;
}

ServerConfig& ServerConfig::getInstance(){
	if (_singleton == NULL)
		throw std::invalid_argument("Singleton is not instanciated.");
	return *_singleton;
}

ServerBlock& ServerConfig::findServer(uint32_t port)
{
	std::vector<ServerBlock>::iterator itServer;

	itServer = _servers.begin();
	while (itServer != _servers.end())
	{
		ServerBlock& serv = *itServer;
		std::vector<ServerBlock::Host>::iterator itListen = serv.listens.begin();
		while (itListen != serv.listens.end())
		{
			ServerBlock::Host& listen = *itListen;
			if (listen.port == port)
				return serv;
			itListen++;
		}
		
		itServer++;
	}
	
	return _servers[0];
}

std::vector<uint32_t> ServerConfig::getPorts()
{
	std::vector<uint32_t> ports;
	std::vector<ServerBlock>::iterator itServer;

	for (itServer = _servers.begin(); itServer != _servers.end(); itServer++)
	{
		std::vector<ServerBlock::Host>::iterator itListen;
		for (itListen = itServer->listens.begin(); itListen != itServer->listens.end(); itListen++)
			ports.push_back(itListen->port);
	}
	return ports;
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
	error += '\n';
	throw ServerConfig::SyntaxError(error);
}

void ServerConfig::_skipSemiColonNewLine(parser::config::ScannerConfig & scanner)
{
	pr::Token t;

	if ((t = scanner.getToken()).kind != pr::ScopedEnum::kSemiColon)
		_throw_SyntaxError(t, "Missing semi-colon.");
	if ((t = scanner.getToken()).kind != pr::ScopedEnum::kNewLine)
		if (t.kind != pr::ScopedEnum::kComment)
		_throw_SyntaxError(t, "Missing new line after semi-colon.");
}


void ServerConfig::_parse(std::istream & in)
{
	pr::ScannerConfig scanner(in);

	pr::Token t;
	while ((t = scanner.getToken(true)).kind != pr::ScopedEnum::kEndOfInput)
	{
		switch (t.kind)
		{
			case pr::ScopedEnum::kComment:
				continue;
			case pr::ScopedEnum::kString:
				if (t.kind== pr::ScopedEnum::kString && t.value == "server")
					_servers.push_back(_parseServer(scanner));
				else
				_throw_SyntaxError(t,
					"Unknown directive \"" + t.value + "\" at root context");
	
				break;
			default:
				_throw_SyntaxError(t, "Unexpected token: " + pr::tokenToString(t));
		}
	}
}

ServerBlock ServerConfig::_parseServer(pr::ScannerConfig & scanner)
{
	ServerBlock result;
	pr::Token t;

	result.autoindex = false;
	if ((t = scanner.getToken(true)).kind != pr::ScopedEnum::kLeftBrace)
		_throw_SyntaxError(t, "Missing open brace at server block.");
	while ((t = scanner.getToken(true)).kind != pr::ScopedEnum::kEndOfInput
	 && t.kind != pr::ScopedEnum::kRightBrace)
	{
		switch (t.kind)
		{
			case pr::ScopedEnum::kComment :
				continue;
			case pr::ScopedEnum::kString :
				if (t.value == "listen")
					result.listens.push_back(_parseListen(scanner));
				else if (t.value == "root")
					result.root = _parseRoot(scanner);
				else if (t.value == "index")
					result.index = _parseIndex(scanner);
				else if (t.value == "server_name")
					_parseServerName(scanner);
				else if (t.value == "error_page")
				{
					std::map<u_short, std::string> m = _parseErrorPage(scanner);
					result.errors.insert(m.begin(), m.end());
				}
				else if (t.value == "location")
					result.locations.push_back(_parseLocation(scanner));
				else if (t.value == "autoindex")
					result.autoindex = _parseAutoindex(scanner);
				else
					_throw_SyntaxError(t,
						"Unknown directive \"" + t.value + "\" in context 'server'");
				break;
			case pr::ScopedEnum::kSemiColon :
				_throw_SyntaxError(t, "Unexpected semi-colon.");
				break;
			default:
				_throw_SyntaxError(t, "Unexpected token:  " + pr::tokenToString(t));
				break;
		}
	}
	return result;
}

ServerBlock::Host ServerConfig::_parseListen(parser::config::ScannerConfig & scanner)
{
	ServerBlock::Host result;
	pr::Token t =  scanner.getToken();

	if (t.kind != pr::ScopedEnum::kString && t.kind != pr::ScopedEnum::kInteger)
		_throw_SyntaxError(t, "Bad character in \"listen\" context.");
	result = _parseListenValue(t);
	_skipSemiColonNewLine(scanner);
	return result;
}

std::string ServerConfig::_parseRoot(parser::config::ScannerConfig & scanner)
{
	pr::Token t;

	if ((t = scanner.getToken()).kind != pr::ScopedEnum::kString)
		_throw_SyntaxError(t, "Bad token in root context");
	_skipSemiColonNewLine(scanner);
	return t.value;
}

std::string ServerConfig::_parseIndex(parser::config::ScannerConfig & scanner)
{
	pr::Token t;

	if ((t = scanner.getToken()).kind != pr::ScopedEnum::kString)
		_throw_SyntaxError(t, "Bad token in index context");
	_skipSemiColonNewLine(scanner);
	return t.value;
}

std::string ServerConfig::_parseServerName(parser::config::ScannerConfig & scanner)
{
	pr::Token t;

	if ((t = scanner.getToken()).kind != pr::ScopedEnum::kString)
		_throw_SyntaxError(t, "Bad token in server_name context");
	_skipSemiColonNewLine(scanner);
	return t.value;	
}

std::map<u_short, std::string> ServerConfig::_parseErrorPage(parser::config::ScannerConfig & scanner)
{
	std::map<u_short, std::string> result;
	std::vector<u_short> codes;
	std::string			path;
	pr::Token			t;
	
	t = scanner.getToken();
	if (t.kind != pr::ScopedEnum::kInteger)
		_throw_SyntaxError(t, "No error code specified.");
	do {
		codes.push_back(strtoul(t.value.c_str(), 0, 10));
	} while ((t = scanner.getToken()).kind == pr::ScopedEnum::kInteger);
	
	if (t.kind == pr::ScopedEnum::kString)
		path = t.value;
	else
		_throw_SyntaxError(t, "Bad token in context \"error\".");
	std::vector<u_short>::iterator it = codes.begin();
	while (it != codes.end())
	{
		result[*it] = path;
		it++;	
	}
	_skipSemiColonNewLine(scanner);
	return result;
	
}

ServerBlock::Location ServerConfig::_parseLocation(pr::ScannerConfig & scanner)
{
	pr::Token	t;
	ServerBlock::Location result;
	
	if ((t = scanner.getToken()).kind != pr::ScopedEnum::kString)
		_throw_SyntaxError(t, "Location directive: invalid uri");
	result.uri = t.value;
	result.autoindex = false;
	if ((t = scanner.getToken()).kind != pr::ScopedEnum::kLeftBrace)
		_throw_SyntaxError(t, "Location directive: No scope defined. Add braces...");

	while ((t = scanner.getToken(true)).kind != pr::ScopedEnum::kEndOfInput
		&& t.kind != pr::ScopedEnum::kRightBrace)
	{
		switch (t.kind)
		{
			case pr::ScopedEnum::kComment :
				continue;
			case pr::ScopedEnum::kString :
				if (t.value == "include"){
					scanner.getToken();
					_skipSemiColonNewLine(scanner);
				}
				else if (t.value == "fastcgi_pass")	{
					scanner.getToken();
					_skipSemiColonNewLine(scanner);
				}
				else if (t.value == "fastcgi_param")
				{
					scanner.getToken();
					scanner.getToken();
					_skipSemiColonNewLine(scanner);
				}
				else if (t.value == "root")
					result.root = _parseRoot(scanner);
				else if (t.value == "index")
					result.index = _parseIndex(scanner);
				else if (t.value == "autoindex")
					result.autoindex = _parseAutoindex(scanner);
				else
					_throw_SyntaxError(t,
						"Unknown directive \"" + t.value + "\" in location context");
				
				break;
			default:
					_throw_SyntaxError(t, "Unexpected token: " + pr::tokenToString(t));
		}
	}
	return result;
}

bool	ServerConfig::_parseAutoindex(parser::config::ScannerConfig & scanner)
{
	pr::Token t;

	if ((t = scanner.getToken()).kind != pr::ScopedEnum::kString)
		_throw_SyntaxError(t, "Bad token in context \"autoindex\".");
	_skipSemiColonNewLine(scanner);
	if (t.value == "on")
		return true;
	if (t.value == "off")
		return false;
	_throw_SyntaxError(t, "Invalid value in \"autoindex\", must be either \"on\" or \"off\".");
	return false;
}


ServerBlock::Host ServerConfig::_parseListenValue(const pr::Token& host)
{
	ServerBlock::Host result;
	std::string tmp;
	u_short port = 0;

    std::string::const_iterator it = host.value.begin();
    std::string::const_iterator end = host.value.end();

    while(it != end && *it != ':')
		tmp += *it++;
	
	if (it == end)
	{
		it = tmp.begin();
		end = tmp.end();
	}
	else
	{
		result.host = tmp;
		it++;
	}
	while (it != end)
	{
		if (!isdigit(*it))
			_throw_SyntaxError(host, "No port defined in listen directive.");
		port = port * 10 + *it - '0';
		it++;
	}
    lowerStringInPlace(result.host);
	result.port = port;
	return result;
}

ServerBlock::Host ServerConfig::_parseHost(const pr::Token& host)
{
	ServerBlock::Host result;
	(void)host;

	return result;
}
