/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 15:01:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/20 19:50:35 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/ServerConfig.hpp"

#include "parser/config/ScannerConfig.hpp"
#include "utility.hpp"
#include  "SyntaxError.hpp"


#include <fstream>
#include <exception>
#include <cstdlib>

ServerConfig* ServerConfig::_singleton = NULL;

namespace pr = parser::config;

ServerConfig::ServerConfig(const std::string& filepath)
	: _configFilePath(filepath)
{
	std::ifstream file;

	file.open(filepath.c_str(), std::ifstream::in);
	if (!file.is_open())
		throw std::invalid_argument("Can't open file");
	_parse(file);
	_postParser();
	file.close();
}

ServerConfig& ServerConfig::getInstance(std::string filepath){
	if (_singleton == NULL)
		_singleton = new ServerConfig(filepath);
	return *_singleton;
}

ServerConfig& ServerConfig::getInstance(){
	if (_singleton == NULL)
		throw std::invalid_argument("ServerConfig singleton is not instanciated. Must give filepath.");
	return *_singleton;
}

ServerBlock& ServerConfig::findServer(uint32_t port)
{
	std::vector<ServerBlock>::iterator itServer;

	itServer = _servers.begin();
	while (itServer != _servers.end())
	{
		ServerBlock& serv = *itServer;
		std::vector<Host>::iterator itListen = serv.getListens().begin();
		while (itListen != serv.getListens().end())
		{
			Host& listen = *itListen;
			if (listen.getPort() == port)
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
		std::vector<Host>::iterator itListen;
		for (itListen = itServer->getListens().begin(); itListen != itServer->getListens().end(); itListen++)
			ports.push_back(itListen->getPort());
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
	throw SyntaxError(error);
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

void ServerConfig::_postParser(){
	_postParserSetAutoindexInChilds();
	_postParserSetClientMaxBodySizeInChilds();
}

void ServerConfig::_postParserSetAutoindexInChilds(){
	std::vector<ServerBlock>::iterator itServer;

	for (itServer = _servers.begin(); itServer != _servers.end(); itServer++)
	{
		if (itServer->hasAutoindex() == false)
			continue;
		std::vector<Location>::iterator itLocation;
		for (itLocation = itServer->getLocations().begin(); itLocation != itServer->getLocations().end(); itLocation++)
			if (itLocation->hasAutoindex() == false)
				itLocation->setAutoindex(itServer->getAutoindex());
	}
}

void ServerConfig::_postParserSetClientMaxBodySizeInChilds(){
	std::vector<ServerBlock>::iterator itServer;

	for (itServer = _servers.begin(); itServer != _servers.end(); itServer++)
	{
		if (itServer->hasClientMaxBodySize() == false)
			continue;
		std::vector<Location>::iterator itLocation;
		for (itLocation = itServer->getLocations().begin(); itLocation != itServer->getLocations().end(); itLocation++)
			if (itLocation->hasClientMaxBodySize() == false)
				itLocation->setClientMaxBodySize(itServer->getClientMaxBodySize());
	}
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
					"Unknown directive \"" + t.value + "\" in main context");
	
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
					result.addListen(_parseListen(scanner));
				else if (t.value == "root")
					result.setRoot(_parseRoot(scanner));
				else if (t.value == "index")
					result.setIndex(_parseIndex(scanner));
				else if (t.value == "server_name")
					result.setServerName(_parseServerName(scanner));
				else if (t.value == "error_page")
					result.addErrors(_parseErrorPage(scanner));
				else if (t.value == "location")
					result.addLocation(_parseLocation(scanner));
				else if (t.value == "autoindex")
					result.setAutoindex(_parseAutoindex(scanner));
				else if (t.value == "client_max_body_size")
					result.setClientMaxBodySize(_parseClientMaxBodySize(scanner));
				else if (t.value == "return")
					result.setReturnDirective(_parseReturn(scanner));
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

Host ServerConfig::_parseListen(parser::config::ScannerConfig & scanner)
{
	Host result;
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

Location ServerConfig::_parseLocation(pr::ScannerConfig & scanner)
{
	pr::Token	t;
	Location result;
	
	if ((t = scanner.getToken()).kind != pr::ScopedEnum::kString)
		_throw_SyntaxError(t, "Location directive: invalid uri");
	result.setUri(t.value);
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
				else if (t.value == "fastcgi_pass")
					result.setFastCgiPass(_parseHost(scanner));
				else if (t.value == "fastcgi_param")
					result.addFastCgiParam(_parseFastCgiParam(scanner));
				else if (t.value == "root")
					result.setRoot(_parseRoot(scanner));
				else if (t.value == "index")
					result.setIndex(_parseIndex(scanner));
				else if (t.value == "autoindex")
					result.setAutoindex(_parseAutoindex(scanner));
				else if (t.value == "client_max_body_size")
					result.setClientMaxBodySize(_parseClientMaxBodySize(scanner));
				else if (t.value == "return")
					result.setReturnDirective(_parseReturn(scanner));
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


Host ServerConfig::_parseListenValue(const pr::Token& host)
{
	Host result;
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
	    lowerStringInPlace(tmp);
		result.setHostname(tmp);
		it++;
	}
	while (it != end)
	{
		if (!isdigit(*it))
			_throw_SyntaxError(host, "No port defined in listen directive.");
		port = port * 10 + *it - '0';
		it++;
	}
	result.setPort(port);
	return result;
}

Host ServerConfig::_parseHost(parser::config::ScannerConfig & scanner)
{
	std::string	host;
	pr::Token	t;
	uint32_t 	port = 0;
	
	if ((t = scanner.getToken()).kind != pr::ScopedEnum::kString)
		_throw_SyntaxError(t, "Invalid value host.");

    std::string::const_iterator it = t.value.begin();
    std::string::const_iterator end = t.value.end();

    while(it != end && *it != ':')
        host += *it++;
    
    if (it != end && *it == ':')
    {
        it++;
        while (it != end && isdigit(*it))
        {
            port = port * 10 + *it - '0';
            it++;
        }
    }
    lowerStringInPlace(host);
	_skipSemiColonNewLine(scanner);
	return Host(host, port);
}

std::pair<std::string, std::string>	ServerConfig::_parseFastCgiParam(parser::config::ScannerConfig & scanner)
{
	pr::Token tName;
	pr::Token tValue;
	std::pair<std::string, std::string> result;

	if ((tName = scanner.getToken()).kind != pr::ScopedEnum::kString)
		_throw_SyntaxError(tName, "Bad fastcgi parameter name");
	if ((tValue = scanner.getToken()).kind != pr::ScopedEnum::kString)
		_throw_SyntaxError(tValue, "Bad fastcgi parameter value");
	result.first = tName.value;
	result.second = tValue.value;
	_skipSemiColonNewLine(scanner);
	return result;
}

size_t	ServerConfig::_parseClientMaxBodySize(parser::config::ScannerConfig & scanner)
{
	size_t bytes = 0;
	char unit = 0;
	pr::Token t;

	if ((t = scanner.getToken()).kind != pr::ScopedEnum::kString && t.kind != pr::ScopedEnum::kInteger)
		_throw_SyntaxError(t, "Unexpected token: " + pr::tokenToString(t) + " in context \"client_max_body_size\".");
	_skipSemiColonNewLine(scanner);
	
	if (t.kind == pr::ScopedEnum::kInteger)
		return strtoul(t.value.c_str(), 0, 10);
	
    std::string::const_iterator it = t.value.begin();
    std::string::const_iterator end = t.value.end();

	while (it != end && isdigit(*it))
	{
		bytes = bytes * 10 + *it - '0';
		it++;
	}
	unit = *it;

	switch (unit)
	{
		case 'k':
			bytes *= 1000;
			break;
		case 'K':
			bytes *= 1024;
			break;
		case 'm':
			bytes *= 1000;
			bytes *= 1000;
			break;
		case 'M':
			bytes *= 1024;
			bytes *= 1024;
			break;	
	default:
		_throw_SyntaxError(t, std::string("Unknown unit '") + std::string(it, end) + std::string("' in context \"client_max_body_size\". RTFM !"));
		break;
	}

	return bytes;
}

ReturnDirective	ServerConfig::_parseReturn(parser::config::ScannerConfig & scanner)
{
	ReturnDirective result;
	pr::Token argOne;
	pr::Token argTwo;
	size_t code = 0;

	if ((argOne = scanner.getToken()).kind != pr::ScopedEnum::kString && argOne.kind != pr::ScopedEnum::kInteger)
		_throw_SyntaxError(argOne, "Unexpected token: " + pr::tokenToString(argOne) + " in context \"return\".");

	if (argOne.kind == pr::ScopedEnum::kInteger)
	{
		if ((argTwo = scanner.getToken()).kind != pr::ScopedEnum::kString)
			_throw_SyntaxError(argTwo, "Unexpected token: " + pr::tokenToString(argTwo) + " in context \"return\".");

		std::string::const_iterator it = argOne.value.begin();
		std::string::const_iterator end = argOne.value.end();

		while (it != end && isdigit(*it))
		{
			code = code * 10 + *it - '0';	
			it++;
		}
		if (code == 301 || code == 302 || code == 303
		|| code == 307 || code == 308)
		{
			try {
				result.setUri(argTwo.value);}
			catch(const SyntaxError& e){
				_throw_SyntaxError(argTwo, "Problem with uri in context \"return\".");
			}
		}
		else if (code == 204 || code == 400 || code == 402
		|| code == 406 || code == 408 || code == 410 || code == 411
		|| code == 413 || code == 416 || code == 500 || code == 504)
			result.setText(argTwo.value);
		else
			_throw_SyntaxError(argOne, "Can't use code " + intToString(code) + " in context \"return\". RTFM !");
		result.setCode(code);
	}
	else
	{	
		try {
			result.setUri(argOne.value); }
		catch(const SyntaxError& e)	{
			_throw_SyntaxError(argOne, "Problem with uri in context \"return\".");
		}
	}
	_skipSemiColonNewLine(scanner);
	return result;
}
