/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 15:01:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/20 19:34:05 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <vector>
#include <map>
#include <string>
#include <stdint.h>

#include "Uri.hpp"
#include "parser/config/ScannerConfig.hpp"

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
	Host() :  _port(0) {} 
	Host(std::string hostname, uint32_t port) : _hostname(hostname), _port(port) {} 
	Host(uint32_t port) : _port(port) {} 
	Host(std::string hostname) : _hostname(hostname), _port(0) {} 

	inline std::string	getHostname() const	{ return _hostname; }
	inline uint32_t		getPort() const		{ return _port; }

	void setHostname(std::string hostname)	{ _hostname = hostname; }
	void setPort(uint32_t port) 			{ _port = port; }

private:
	std::string _hostname;
	uint32_t	_port;
}; /* class Host */

class Location
{
public:
	Location(): _autoindex(false), _hasAutoindex(false), _clientMaxBodySize(0) {}

	inline std::string	getUri() const				{ return _uri; }
	inline bool			getAutoindex() const		{ return _autoindex; }
	inline bool			hasAutoindex() const		{ return _hasAutoindex; }
	inline Host			getFastCgiPass() const		{ return _fastCgiPass; }
	inline size_t		getClientMaxBodySize() const{ return _clientMaxBodySize; }
	inline std::string	getRoot() const				{ return _root; }
	inline std::string	getIndex() const			{ return _index; }

	void 	setUri(std::string uri)				{ _uri = uri; }
	void 	setAutoindex(bool autoindex) {
		 _autoindex = autoindex;
		 _hasAutoindex = true;
	}
	void 	setFastCgiPass(Host host)			{ _fastCgiPass = host;}
	void 	setClientMaxBodySize(size_t size)	{_clientMaxBodySize = size;}
	void 	setRoot(std::string root)			{_root = root;}
	void 	setIndex(std::string index)			{_index = index;}

	void	addFastCgiParam(const std::string& name, const std::string& value) { _fastCgiParams[name] = value;}
	void	addFastCgiParam(const std::pair<std::string,std::string> pair) 	{ _fastCgiParams[pair.first] = pair.second;}

	/// return the map of fastcgiParam (usefull for testing purpose)
	inline std::map<std::string, std::string>&	getFastCgiParams() 	{ return _fastCgiParams;}
	inline std::string	getFastCgiParam(std::string param) 			{ return _fastCgiParams[param];}


private:
	std::string							_uri;
	bool								_autoindex;
	bool								_hasAutoindex;
	Host								_fastCgiPass;
	size_t								_clientMaxBodySize;
	std::map<std::string, std::string> 	_fastCgiParams;
	std::string							_root;
	std::string							_index;
}; /* class Location */

struct ServerBlock
{
public:
	ServerBlock(): _autoindex(false), _hasAutoindex(false) {}

	inline std::string	getIndex() const			{ return _index; }
	inline bool			getAutoindex() const		{ return _autoindex; }
	inline bool			hasAutoindex() const		{ return _hasAutoindex; }
	inline std::string	getRoot() const				{ return _root; }
	inline std::string	getServerName() const		{ return _serverName; }

	/// return listen from given index (usefull for testing purpose)
	inline const Host&				getListen(uint32_t index) const	{ return _listens[index];}
	/// return the vector of listens (usefull for testing purpose)
	inline std::vector<Host>&		getListens() 			{ return _listens;}
	/// return the vector of locations (usefull for testing purpose)
	inline std::vector<Location>&	getLocations() 			{ return _locations;}
	/// return the map of errors (usefull for testing purpose)
	inline std::map<u_short, std::string>&	getErrors() 	{ return _errors;}

	void	addListen(Host listen) {_listens.push_back(listen);}
	void	addLocation(Location location) {_locations.push_back(location);}
	void	addError(u_short code, const std::string& path) {_errors[code] = path;}
	void	addErrors(const std::map<u_short, std::string>& errors) { _errors.insert(errors.begin(), errors.end());}
	
	void 	setAutoindex(bool autoindex) {
		 _autoindex = autoindex;
		 _hasAutoindex = true;
	}
	void 	setIndex(std::string index)				{ _index = index;}
	void 	setRoot(std::string root)				{ _root = root;}
	void 	setServerName(std::string serverName)	{ _serverName = serverName;}

private:
	
	std::vector<Host>					_listens;
	std::vector<Location>				_locations;
	std::map<u_short, std::string> 		_errors;
	std::string							_index;
	bool								_autoindex;
	bool								_hasAutoindex;
	std::string							_serverName;
	std::string							_root;
}; /* class ServerBlock */

/// @brief Singleton Class
class ServerConfig
{
public:

	/// Instanciate the singleton instance at first call with given file
	static ServerConfig&	getInstance(std::string filepath);
	/// Must instanciate the class before using this function.
	static ServerConfig&	getInstance();
	inline std::string		getConfigFilePath() const { return _configFilePath;}

	/// return the vector of servers (usefull for testing)
	inline std::vector<ServerBlock>	getServers() const			{ return _servers;}
	/// return server from given index (usefull for testing purpose)
	inline ServerBlock&				getServer(uint32_t index) 	{ return _servers[index];}
	/// return the server who own the given port
	ServerBlock&					findServer(uint32_t port);

	/// return all ports defined in listen directives.
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

	void _postParser();

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
