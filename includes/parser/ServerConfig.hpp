/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 15:01:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/20 16:54:18 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEREVER_CONFIG_HPP
#define SEREVER_CONFIG_HPP

#include <vector>
#include <map>
#include <string>
#include <cctype>

#include "Uri.hpp"
#include "parser/config/ScannerConfig.hpp"

// Singleton


/* Syntax:	 autoindex on | off;
 * Default: autoindex off;
 * Context: http, server, location
 * http://nginx.org/en/docs/http/ngx_http_autoindex_module.html
 *
 *	Syntax:	client_max_body_size size;
 * Default: client_max_body_size 1m;
 * Context:	http, server, location
 * http://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size
 *
 */

class Host
{
public:
	inline std::string	gethostname() const	{ return _hostname; }
	inline uint32_t		getPort() const		{ return _port; }

	void sethostname(std::string hostname)	{ _hostname = hostname; }
	void setPort(uint32_t port) 			{ _port = port; }

private:
	std::string _hostname;
	uint32_t	_port;
}; /* class Host */

class Location
{
public:
	inline std::string	getUri() const				{ return _uri; }
	inline bool		getAutoIndex() const			{ return _autoindex; }
	inline Host		getFastCgiPass() const			{ return _fastCgiPass; }
	inline size_t	getClientMaxBodySize() const	{ return _clientMaxBodySize; }
	inline std::string	getRoot() const				{ return _root; }
	inline std::string	getIndex() const			{ return _index; }

	void 	setUri(std::string uri)				{ _uri = uri; }
	void 	setAutoIndex(bool autoindex)		{_autoindex = autoindex;}
	void 	setFastCgiPass(Host host)			{ _fastCgiPass = host;}
	void 	setClientMaxBodySize(size_t size)	{_clientMaxBodySize = size;}
	void 	setRoot(std::string root)			{_root = root;}
	void 	setIndex(std::string index)			{_index = index;}

private:
	std::string							_uri;
	bool								_autoindex;
	Host								_fastCgiPass;
	size_t								_clientMaxBodySize;
	std::map<std::string, std::string> 	_fatsCgiParam;
	std::string							_root;
	std::string							_index;
}; /* class Location */

struct ServerBlock
{
public:
	inline std::string	getIndex() const			{ return _index; }
	inline bool			getAutoIndex() const		{ return _autoindex; }
	inline std::string	getRoot() const				{ return _root; }
	inline std::string	getServerName() const			{ return _serverName; }

	// return listen from given server (usefull for testing purpose)
	inline const Host&			getListen(uint32_t index) const	{ return _listens[index];}
	inline std::vector<Host>&	getListens() 			{ return _listens;}

	void 	setAutoIndex(bool autoindex)			{ _autoindex = autoindex;}
	void 	setIndex(std::string index)				{ _index = index;}
	void 	setRoot(std::string root)				{ _root = root;}
	void 	setServerName(std::string serverName)	{ _serverName = serverName;}



	std::vector<Location>				locations;
	std::map<u_short, std::string> 		errors;

private:
	
	std::vector<Host>					_listens;
	std::string							_index;
	bool								_autoindex;
	std::string							_serverName;
	std::string							_root;
}; /* class ServerBlock */

class ServerConfig
{
public:

	static ServerConfig&	getInstance(std::string filepath);
	static ServerConfig&	getInstance();
	inline std::string		getConfigFilePath() const { return _configFilePath;}

	// return the vector of servers (usefull for testing)
	inline std::vector<ServerBlock>	getServers() const				{ return _servers;}
	// return server from given index (usefull for testing purpose)
	inline ServerBlock&		getServer(uint32_t index) 	{ return _servers[index];}
	// return the server who own the given port
	ServerBlock&					findServer(uint32_t port);

	// return all ports defined in listen directives.
	std::vector<uint32_t>			getPorts();

private:
	ServerConfig(const std::string & filepath);
	ServerConfig(ServerConfig&);
	ServerConfig& operator=(const ServerConfig&);
	
	void								_parse(std::istream &);

	ServerBlock							_parseServer(parser::config::ScannerConfig & scanner);
	Host								_parseListen(parser::config::ScannerConfig & scanner);
	std::string 						_parseRoot(parser::config::ScannerConfig & scanner);
	std::string 						_parseIndex(parser::config::ScannerConfig & scanner);
	bool								_parseAutoindex(parser::config::ScannerConfig & scanner);
	std::string 						_parseServerName(parser::config::ScannerConfig & scanner);
	std::map<u_short, std::string>		_parseErrorPage(parser::config::ScannerConfig & scanner);
	Location							_parseLocation(parser::config::ScannerConfig & scanner);
	Host 								_parseHost(parser::config::ScannerConfig & scanner);
	std::pair<std::string, std::string>	_parseFastCgiParam(parser::config::ScannerConfig & scanner);
	
	Host _parseListenValue(const parser::config::Token& host);

	void _skipSemiColonNewLine(parser::config::ScannerConfig & scanner);
	void _throw_SyntaxError(parser::config::Token t, const std::string &error_str);

	static ServerConfig*		_singleton;
	std::vector<ServerBlock>	_servers;
	std::string					_configFilePath;

public:

	class SyntaxError: public std::exception
	{
		public:
			SyntaxError(const char * message)		: _message(message) {}
			SyntaxError(const std::string& message)	: _message(message) {}
			virtual ~SyntaxError() throw() {}

			virtual const char* what() const throw() {
				return _message.c_str();
			}

		protected:
			std::string _message;
	};

}; /* class ServerConfig */

#endif /* SERVER_CONFIG_HPP */
