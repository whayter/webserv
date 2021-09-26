/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 15:01:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/09/12 16:25:06 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"

#include "config/ScannerConfig.hpp"
#include "utility.hpp"
#include "SyntaxError.hpp"


#include <fstream>
#include <exception>
#include <cstdlib>

ServerConfig* ServerConfig::_singleton = NULL;

namespace pr = config;

ServerConfig::ServerConfig(const ft::filesystem::path& filepath)
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

ServerConfig& ServerConfig::getInstance(ft::filesystem::path filepath){
	if (_singleton == NULL)
		_singleton = new ServerConfig(filepath);
	return *_singleton;
}

ServerConfig& ServerConfig::getInstance(){
	if (_singleton == NULL)
		throw std::invalid_argument("ServerConfig singleton is not instanciated. Must give filepath.");
	return *_singleton;
}

/// ONLY FOR TESTING PURPOSE
void	ServerConfig::__delete_singleton_instance()
{
	if (_singleton != NULL)
	{
		delete _singleton;
		_singleton = NULL;
	}
}

// ServerBlock& ServerConfig::findServer(uint32_t port)
// {
// 	std::vector<ServerBlock>::iterator itServer;

// 	itServer = _servers.begin();
// 	while (itServer != _servers.end())
// 	{
// 		ServerBlock& serv = *itServer;
// 		std::vector<Host>::iterator itListen = serv.getListens().begin();
// 		while (itListen != serv.getListens().end())
// 		{
// 			Host& listen = *itListen;
// 			if (listen.getPort() == port)
// 				return serv;
// 			itListen++;
// 		}
		
// 		itServer++;
// 	}
	
// 	return _servers[0];
// }

ServerBlock& ServerConfig::findServer(const Uri& uri)
{
	std::vector<ServerBlock>::reverse_iterator itServer;
	ServerBlock& bestMatch = _servers[0];

	for (itServer = _servers.rbegin(); itServer != _servers.rend(); itServer++)
	{
		std::vector<Host>::iterator itListen = itServer->getListens().begin();
		for (; itListen != itServer->getListens().end(); itListen++)
		{
			if (itListen->getPort() == uri.getPort())
			{
				bestMatch = *itServer;
				if (itServer->getServerName() == uri.getHost())
					return bestMatch;
			}
		}
	}
	return bestMatch;
}

const Location& ServerConfig::findLocation(const Uri& uri)
{
	return findServer(uri).findLocation(uri);
}

ft::filesystem::path	ServerConfig::getPathFromUri(const Uri& uri)
{
	return findServer(uri).getPathFromUri(uri);
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



void ServerConfig::_throw_SyntaxError(config::Token t, const std::string &error_str)
{
	std::string error;

	error += _configFilePath.string() + ':';
	error += ft::intToString(t.line);
	error += ':';
	error += ft::intToString(t.column);
	error += ": error: ";
	error += error_str;
	error += '\n';
	throw SyntaxError(error);
}

void ServerConfig::_skipSemiColonNewLine(config::ScannerConfig & scanner)
{
	pr::Token t;

	if ((t = scanner.getToken()).kind != pr::TokenKind::kSemiColon)
		_throw_SyntaxError(t, "Missing semi-colon.");
	if ((t = scanner.getToken()).kind != pr::TokenKind::kNewLine)
		if (t.kind != pr::TokenKind::kComment)
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
	while ((t = scanner.getToken(true)).kind != pr::TokenKind::kEndOfInput)
	{
		switch (t.kind.getValue())
		{
			case pr::TokenKind::kComment:
				continue;
			case pr::TokenKind::kString:
				if (t.value == "server")
					_servers.push_back(_parseServer(scanner, t));
				else
				_throw_SyntaxError(t,
					"Unknown directive \"" + t.value + "\" in main context");
	
				break;
			default:
				_throw_SyntaxError(t, "Unexpected token: " + pr::tokenToString(t));
		}
	}
}

ServerBlock ServerConfig::_parseServer(pr::ScannerConfig & scanner, pr::Token serverToken)
{
	ServerBlock result;
	pr::Token t;

	if ((t = scanner.getToken(true)).kind != pr::TokenKind::kLeftBrace)
		_throw_SyntaxError(t, "Missing open brace at server block.");
	while ((t = scanner.getToken(true)).kind != pr::TokenKind::kRightBrace)
	{
		switch (t.kind.getValue())
		{
			case pr::TokenKind::kComment :
				continue;
			case pr::TokenKind::kString :
				if (t.value == "server")
					_throw_SyntaxError(serverToken, "Missing closing bracket at end of server directive");
				else if (t.value == "listen")
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
					result.addLocation(_parseLocation(scanner, t));
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
			case pr::TokenKind::kSemiColon :
				_throw_SyntaxError(t, "Unexpected semi-colon.");
				break;
			case pr::TokenKind::kEndOfInput :
				_throw_SyntaxError(serverToken, "Missing closing bracket at end of server directive");
				break;
			default:
				_throw_SyntaxError(t, "Unexpected token:  " + pr::tokenToString(t));
				break;
		}
	}
	return result;
}

Host ServerConfig::_parseListen(config::ScannerConfig & scanner)
{
	Host result;
	pr::Token t =  scanner.getToken();

	if (t.kind != pr::TokenKind::kString && t.kind != pr::TokenKind::kInteger)
		_throw_SyntaxError(t, "Bad token " + pr::tokenToString(t) + "in context \"listen\".");
	result = _parseListenValue(t);
	_skipSemiColonNewLine(scanner);
	return result;
}

ft::filesystem::path ServerConfig::_parseRoot(config::ScannerConfig & scanner)
{
	pr::Token t;

	if ((t = scanner.getToken()).kind != pr::TokenKind::kString)
		_throw_SyntaxError(t, "Bad token " + pr::tokenToString(t) + "in context \"root\".");
	_skipSemiColonNewLine(scanner);
	return ft::filesystem::path(t.value);
}

ft::filesystem::path ServerConfig::_parseIndex(config::ScannerConfig & scanner)
{
	pr::Token t;

	if ((t = scanner.getToken()).kind != pr::TokenKind::kString)
		_throw_SyntaxError(t, "Bad token " + pr::tokenToString(t) + "in context \"index\".");
	_skipSemiColonNewLine(scanner);
	return ft::filesystem::path(t.value);
}

std::string ServerConfig::_parseServerName(config::ScannerConfig & scanner)
{
	pr::Token t;

	if ((t = scanner.getToken()).kind != pr::TokenKind::kString)
		_throw_SyntaxError(t, "Bad token " + pr::tokenToString(t) + "in context \"server_name\".");
	_skipSemiColonNewLine(scanner);
	return t.value;	
}

std::map<u_short, std::string> ServerConfig::_parseErrorPage(config::ScannerConfig & scanner)
{
	std::map<u_short, std::string> result;
	std::vector<u_short> codes;
	std::string			path;
	pr::Token			t;
	
	t = scanner.getToken();
	if (t.kind != pr::TokenKind::kInteger)
		_throw_SyntaxError(t, "No error code specified.");
	do {
		codes.push_back(strtoul(t.value.c_str(), 0, 10));
	} while ((t = scanner.getToken()).kind == pr::TokenKind::kInteger);
	
	if (t.kind == pr::TokenKind::kString)
		path = t.value;
	else if (t.kind == pr::TokenKind::kSemiColon)
		_throw_SyntaxError(t, "Missing Uri in context \"error\".");
	else
		_throw_SyntaxError(t, "Bad token " + pr::tokenToString(t) + "in context \"error\".");
	std::vector<u_short>::iterator it = codes.begin();
	while (it != codes.end())
	{
		result[*it] = path;
		it++;	
	}
	_skipSemiColonNewLine(scanner);
	return result;
	
}

Location ServerConfig::_parseLocation(pr::ScannerConfig & scanner, pr::Token locationToken)
{
	pr::Token	t;
	Location result;
	
	{
		pr::Token	t2;
		if ((t = scanner.getToken()).kind != pr::TokenKind::kString)
			_throw_SyntaxError(t, "Location directive: invalid first argument. RTFM");
		if ((t2 = scanner.getToken()).kind == pr::TokenKind::kLeftBrace)
		{
			result.setUri(t.value);
			t = t2;
		}
		else if (t2.kind == pr::TokenKind::kString)
		{
			result.setExtentionFile(t2.value);
			t = scanner.getToken();
		}
		else
			_throw_SyntaxError(t2, "Location directive: invalid second argument. RTFM");
	}
	if (t.kind != pr::TokenKind::kLeftBrace)
		_throw_SyntaxError(t, "Location directive: No scope defined. Add braces...");

	while ((t = scanner.getToken(true)).kind != pr::TokenKind::kRightBrace)
	{
		switch (t.kind.getValue())
		{
			case pr::TokenKind::kComment :
				continue;
			case pr::TokenKind::kString :
				if (t.value == "include"){
					scanner.getToken();
					_skipSemiColonNewLine(scanner);
				}
				else if (t.value == "location")
					_throw_SyntaxError(locationToken, "Missing closing bracket at end of location directive");
				// else if (t.value == "fastcgi_pass")
				// 	result.setFastCgiPass(_parseHost(scanner));
				else if (t.value == "cgi_exec")
					result.setCgiExec(_parseCgiExec(scanner));
				else if (t.value == "cgi_param")
					result.addCgiParam(_parseCgiParam(scanner));
				else if (t.value == "root")
					result.setRoot(_parseRoot(scanner));
				else if (t.value == "index")
					result.setIndex(_parseIndex(scanner));
				else if (t.value == "autoindex")
					result.setAutoindex(_parseAutoindex(scanner));
				else if (t.value == "client_max_body_size")
					result.setClientMaxBodySize(_parseClientMaxBodySize(scanner));
				else if (t.value == "limit_except")
					result.addLimitExceptMethods(_parseLimitExceptMethods(scanner));
				else if (t.value == "return")
					result.setReturnDirective(_parseReturn(scanner));
				else
					_throw_SyntaxError(t,
						"Unknown directive \"" + t.value + "\" in location context");
				break;
			case pr::TokenKind::kEndOfInput :
				_throw_SyntaxError(locationToken, "Missing closing bracket at end of location directive");
				break;
			default:
				_throw_SyntaxError(t, "Unexpected token: " + pr::tokenToString(t));
		}
	}
	return result;
}

bool	ServerConfig::_parseAutoindex(config::ScannerConfig & scanner)
{
	pr::Token t;

	if ((t = scanner.getToken()).kind != pr::TokenKind::kString)
		_throw_SyntaxError(t, "Bad token " + pr::tokenToString(t) + "in context \"autoindex\".");
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
	    ft::lowerStringInPlace(tmp);
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

Host ServerConfig::_parseHost(config::ScannerConfig & scanner)
{
	std::string	host;
	pr::Token	t;
	uint32_t 	port = 0;
	
	if ((t = scanner.getToken()).kind != pr::TokenKind::kString)
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
    ft::lowerStringInPlace(host);
	_skipSemiColonNewLine(scanner);
	return Host(host, port);
}

std::string ServerConfig::_parseCgiExec(config::ScannerConfig & scanner)
{
	std::string	exec;
	pr::Token	t;

	if ((t = scanner.getToken()).kind != pr::TokenKind::kString)
		_throw_SyntaxError(t, "Invalid value exec.");
	exec = t.value;
	_skipSemiColonNewLine(scanner);
	return exec;
}

std::pair<std::string, std::string>	ServerConfig::_parseCgiParam(config::ScannerConfig & scanner)
{
	pr::Token tName;
	pr::Token tValue;
	std::pair<std::string, std::string> result;

	if ((tName = scanner.getToken()).kind != pr::TokenKind::kString)
		_throw_SyntaxError(tName, "Bad cgi parameter name");
	if ((tValue = scanner.getToken()).kind != pr::TokenKind::kString)
		_throw_SyntaxError(tValue, "Bad cgi parameter value");
	result.first = tName.value;
	result.second = tValue.value;
	_skipSemiColonNewLine(scanner);
	return result;
}

size_t	ServerConfig::_parseClientMaxBodySize(config::ScannerConfig & scanner)
{
	size_t bytes = 0;
	char unit = 0;
	pr::Token t;

	if ((t = scanner.getToken()).kind != pr::TokenKind::kString && t.kind != pr::TokenKind::kInteger)
		_throw_SyntaxError(t, "Unexpected token: " + pr::tokenToString(t) + " in context \"client_max_body_size\".");
	_skipSemiColonNewLine(scanner);
	
	if (t.kind == pr::TokenKind::kInteger)
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

ReturnDirective	ServerConfig::_parseReturn(config::ScannerConfig & scanner)
{
	ReturnDirective result;
	pr::Token argOne;
	pr::Token argTwo;
	size_t code = 0;

	if ((argOne = scanner.getToken()).kind != pr::TokenKind::kString && argOne.kind != pr::TokenKind::kInteger)
		_throw_SyntaxError(argOne, "Unexpected token: " + pr::tokenToString(argOne) + " in context \"return\".");

	if (argOne.kind == pr::TokenKind::kInteger)
	{
		if ((argTwo = scanner.getToken()).kind != pr::TokenKind::kString)
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
			_throw_SyntaxError(argOne, "Can't use code " + ft::intToString(code) + " in context \"return\". RTFM !");
		result.setCode(code);
	}
	else
	{
		// try {
		// 	result.setUri(argOne.value); }
		// catch(const SyntaxError& e)	{
		// 	_throw_SyntaxError(argOne, "Problem with uri in context \"return\".");
		// }
		// result.setCode(302);
		_throw_SyntaxError(argOne, "Invalid return code \""+ argOne.value +"\" in return context. Must be an integer. Don't quote integers. thx");
	}
	_skipSemiColonNewLine(scanner);
	return result;
}

std::set<std::string> ServerConfig::_parseLimitExceptMethods(config::ScannerConfig & scanner)
{
	std::set<std::string> result;
	// std::set<std::string> allowedMethods = {"GET", "POST", "DELETE"}; // c++98 sucks :@
	std::set<std::string> allowedMethods;
	pr::Token t;
	
	// allowedMethods.insert({"GET", "POST", "DELETE"}); // c++ 98 still suck or am I dumb ?! :@ 
	allowedMethods.insert("GET"); 
	allowedMethods.insert("POST"); 
	allowedMethods.insert("DELETE"); 
	
	t = scanner.getToken();
	if (t.kind == pr::TokenKind::kSemiColon)
		_throw_SyntaxError(t, "No allowed method specified. There is no point of doing this.");
	do {
		if (!allowedMethods.count(t.value))
			_throw_SyntaxError(t, "Unknown method \"" + t.value + "\".");
		result.insert(t.value);
	} while ((t = scanner.getToken()).kind == pr::TokenKind::kString);
	if (t.kind != pr::TokenKind::kSemiColon)
		_throw_SyntaxError(t, "Missing semi-colon.");
	if ((t = scanner.getToken()).kind != pr::TokenKind::kNewLine)
		if (t.kind != pr::TokenKind::kComment)
		_throw_SyntaxError(t, "Missing new line after semi-colon.");
	return result;	
}
