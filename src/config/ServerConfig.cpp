/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 15:01:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/10/26 20:15:38 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"

#include "config/ScannerConfig.hpp"
#include "utility.hpp"
#include "filesystem.h"
#include "SyntaxError.hpp"


#include <fstream>
#include <exception>
#include <cstdlib>
#include <climits>

ServerConfig* ServerConfig::_singleton = NULL;

namespace pr = config;

// #ifdef LINUX
// #define MIME_FILE "/etc/mime.types"
// #else
// #define MIME_FILE "???"
// #endif

ServerConfig::ServerConfig(const ft::filesystem::path& filepath, const ft::filesystem::path& mimePath)
	: _configFilePath(filepath)
{
	std::ifstream file;

	if (!ft::filesystem::exists(filepath))
		throw std::invalid_argument("webserv: \"" + filepath.string() + "\" doesn't exists.\n");
	if (ft::filesystem::is_directory(filepath))
		throw std::invalid_argument("webserv: really ?\n  bro, \"" + filepath.string() + "\" is a directory, dum\n");

	file.open(filepath.c_str(), std::ifstream::in);
	if (!file.is_open())
		throw std::invalid_argument("webserv: Can't open file \"" + filepath.string() + "\"\n");
	_parse(file);
	_postParser();
	file.close();

	// if (mimePath.empty())
	// 	_mime = _parseMimeFile(MIME_FILE);
	// else
	if (!mimePath.empty())
		_mime = _parseMimeFile(mimePath);
}

std::map<std::string, std::string>	ServerConfig::_parseMimeFile(const ft::filesystem::path & path){
	std::ifstream file;
	std::map<std::string, std::string> result;

	file.open(path.c_str(), std::ifstream::in);
	if (!file.is_open())
		throw std::invalid_argument("Can't open file \"" + path.string() + "\"");
	{
		pr::ScannerConfig scanner(file);
		pr::Token t;

		std::string mime;
		while ((t = scanner.getToken()).kind != pr::TokenKind::kEndOfInput)
		{
			switch (t.kind.getValue())
			{
				case pr::TokenKind::kComment:
				case pr::TokenKind::kNewLine:
					mime.clear();
					break;
				case pr::TokenKind::kInteger:
				case pr::TokenKind::kString:
					if (mime.empty())
						mime = t.value;
					else
						result[t.value] = mime;
					break;
				default:
					_throw_SyntaxError(t, "Unexpected token: " + pr::tokenToString(t), path);
			}
		}		
	}
	file.close();
	return result;
}

std::string		ServerConfig::getMime(const std::string& extension)
{
	if (extension.at(0) == '.')
		return _mime[extension.c_str() + 1];
	return _mime[extension];
}


ServerConfig& ServerConfig::getInstance(const ft::filesystem::path& filepath, const ft::filesystem::path& mimepath){
	if (_singleton == NULL)
		_singleton = new ServerConfig(filepath, mimepath);
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
	ServerBlock* bestMatch = &_servers[0];

	for (itServer = _servers.rbegin(); itServer != _servers.rend(); itServer++)
	{
		std::vector<Host>::iterator itListen = itServer->getListens().begin();
		for (; itListen != itServer->getListens().end(); itListen++)
		{
			if (itListen->getPort() == uri.getPort())
			{
				bestMatch = &*itServer;
				if (itServer->getServerName() == uri.getHost())
					return *bestMatch;
			}
		}
	}
	return *bestMatch;
}

const Location& ServerConfig::findLocation(const Uri& uri)
{
	return findServer(uri).findLocation(uri);
}

ft::filesystem::path	ServerConfig::getPathFromUri(const Uri& uri)
{
	return findServer(uri).getPathFromUri(uri).second;
}

std::vector<uint32_t> ServerConfig::getPorts()
{
	std::set<uint32_t> ports;
	std::vector<ServerBlock>::iterator itServer;

	for (itServer = _servers.begin(); itServer != _servers.end(); itServer++)
	{
		std::vector<Host>::iterator itListen;
		for (itListen = itServer->getListens().begin(); itListen != itServer->getListens().end(); itListen++)
			ports.insert(itListen->getPort());
	}
	return std::vector<uint32_t>(ports.begin(), ports.end());;
}


void ServerConfig::_throw_SyntaxError(config::Token t, const std::string &error_str, const ft::filesystem::path& file)
{
	std::string error;

	if (file.empty())
		error += _configFilePath.string() + ':';
	else
		error += file.string() + ':';
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
	_postCheckAtLeastOneServerIsDefined();
	_postParserSetDefaultServerLocation();
	_postParserSetLimitExcept();
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

void ServerConfig::_postParserSetClientMaxBodySizeInChilds()
{
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

void ServerConfig::_postParserSetLimitExcept()
{
	std::vector<ServerBlock>::iterator itServer;

	for (itServer = _servers.begin(); itServer != _servers.end(); itServer++)
	{
		std::vector<Location>::iterator itLocation;
		for (itLocation = itServer->getLocations().begin(); itLocation != itServer->getLocations().end(); itLocation++)
			if (itLocation->getLimitExceptMethods().empty())
				itLocation->addLimitExceptMethod("GET");
	}
}

// if no location provided, set a default one
void ServerConfig::_postParserSetDefaultServerLocation()
{
	std::vector<ServerBlock>::iterator itServer;
	Location defaultLocation;
	defaultLocation.setUri("/");

	bool hasRootLocation;
	for (itServer = _servers.begin(); itServer != _servers.end(); itServer++)
	{
		std::vector<Location>::iterator itLocation;
		hasRootLocation = false;
		for (itLocation = itServer->getLocations().begin(); itLocation != itServer->getLocations().end(); itLocation++)
			if (itLocation->getUri() == "/")
			{
				hasRootLocation = true;
				break;
			}
		if (!hasRootLocation)
			itServer->addLocation(defaultLocation);
	}
}
void ServerConfig::_postCheckAtLeastOneServerIsDefined()
{
	if (_servers.size() == 0)
		std::cout << "webserv: [warning] No server defined in \"" << _configFilePath << "\"" << std::endl;
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
				else if (t.value == "upload_store")
					result.setUploadStore(_parseUploadStore(scanner));
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
	errno = 0;
	do {
		char *nptr;
		unsigned long code = strtoul(t.value.c_str(), &nptr, 10);
		if (code == ULONG_MAX && ft::make_error_code().value() == ft::errc::result_out_of_range)
			_throw_SyntaxError(t, std::string("Overflow in context \"error_page\"... thx bro --'"));
		if (nptr[0])
			_throw_SyntaxError(t, std::string("listen directive: Must be a valid number."));
		if (code > 999)
			_throw_SyntaxError(t, std::string("listen directive: Well as far as I know, http standard doesn't define status with more than 3 digits. Annoying bro, go ahead --' "));
		codes.push_back(code);
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
			if (t.value != "ext")
				_throw_SyntaxError(t, "Location directive: unknown option \"" + t.value + "\".");
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

Host ServerConfig::_parseListenValue(const pr::Token& t)
{
	Host result;
	unsigned long port;

	std::string	tmp;

	std::string::const_iterator it = t.value.begin();
	std::string::const_iterator end = t.value.end();

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
		tmp.clear();
		while (it != end)
			tmp += *it++;
	}
	char *nptr;
	errno = 0;
	port = strtoul(tmp.c_str(), &nptr, 10);
	if (port == ULONG_MAX && ft::make_error_code().value() == ft::errc::result_out_of_range)
		_throw_SyntaxError(t, std::string("Overflow in context \"listen\"... thx bro --'"));
	if (nptr[0])
		_throw_SyntaxError(t, std::string("listen directive: Must be a valid number. (1 to 65535)"));
	if (port == 0)
		_throw_SyntaxError(t, std::string("listen directive: Port 0 is a reserved port. (1 to 65535)"));
	if (port > USHRT_MAX)
		_throw_SyntaxError(t, std::string("listen directive: the port is greater than the max port. (1 to 65535)"));
	result.setPort(static_cast<uint16_t>(port));
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
	if (!ft::filesystem::exists(exec))
		_throw_SyntaxError(t, "Cgi binary does not exists.");
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
	char *unit;
	pr::Token t;

	if ((t = scanner.getToken()).kind != pr::TokenKind::kString && t.kind != pr::TokenKind::kInteger)
	{
		if (t.kind == pr::TokenKind::kSemiColon)
			_throw_SyntaxError(t, "Please provide a value to \"client_max_body_size\".");
		_throw_SyntaxError(t, "Unexpected token: " + pr::tokenToString(t) + " in context \"client_max_body_size\".");
	}
	_skipSemiColonNewLine(scanner);
	
	if (!::isdigit(t.value[0]))
		_throw_SyntaxError(t, std::string("please provide a valid number"));
	errno = 0;
	bytes = strtoul(t.value.c_str(), &unit, 10);
	if (bytes == ULONG_MAX && ft::make_error_code().value() == ft::errc::result_out_of_range)
		_throw_SyntaxError(t, std::string("Overflow in context \"client_max_body_size\"... thx bro --'"));
	if (unit[1])
		_throw_SyntaxError(t, std::string("Unknown unit \"") + unit + std::string("\" in context \"client_max_body_size\". RTFM !"));

	switch (*unit)
	{
		case 0:
			break;
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
		_throw_SyntaxError(t, std::string("Unknown unit '") + std::string(unit) + std::string("' in context \"client_max_body_size\". RTFM !"));
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
		else if (code == 400 || code == 402
		|| code == 406 || code == 408 || code == 410 || code == 411
		|| code == 413 || code == 416 || code == 500 || code == 504)
			result.setText(argTwo.value);
		else if (code != 204)
			_throw_SyntaxError(argOne, "Can't use code " + ft::intToString(code) + " in context \"return\". RTFM !");
		result.setCode(code);
	}
	else
		_throw_SyntaxError(argOne, "Invalid return code \""+ argOne.value +"\" in return context. Must be an integer. Don't quote integers. thx");
	_skipSemiColonNewLine(scanner);
	return result;
}

ft::filesystem::path	ServerConfig::_parseUploadStore(config::ScannerConfig & scanner)
{
	pr::Token t;

	if ((t = scanner.getToken()).kind != pr::TokenKind::kString)
		_throw_SyntaxError(t, "Bad token " + pr::tokenToString(t) + "in context \"upload_store\".");
	_skipSemiColonNewLine(scanner);
	return ft::filesystem::path(t.value);
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
