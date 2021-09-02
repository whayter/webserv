/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 15:01:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/30 18:38:34 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <vector>
#include <map>
#include <set>
#include <string>
#include <stdint.h>

#include "Uri.hpp"
#include "parser/config/ScannerConfig.hpp"

#define DEFAULT_CLIENT_MAX_BODY_SIZE 1000 * 1000 // = 1m

class ReturnDirective
{
public:
	ReturnDirective() : _code(0), _uri() {}
	// ReturnDirective(const ReturnDirective& other) :
	// 	_code(other._code),
	// 	_uri(other._uri),
	// 	_text(other._text)
	// {}

	inline u_short 				getCode() const	{ return _code; }
	inline const Uri&			getUri() const	{ return _uri; }
	inline const std::string& 	getText() const	{ return _text; }

	void setCode(u_short code)				{ _code = code; }
	void setUri(const std::string& uri)		{ _uri = uri; }
	void setText(const std::string& text)	{ _text = text; }

	inline bool hasCode() const	{ return _code != 0; }
	inline bool hasUri() const	{ return !_uri.empty(); }
	inline bool hasText() const { return !_text.empty(); }

private:
	u_short 	_code;
	Uri			_uri;
	std::string _text;
}; /* class ReturnDirective */

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
	Location(): _autoindex(false), _hasAutoindex(false), 
		_clientMaxBodySize(DEFAULT_CLIENT_MAX_BODY_SIZE), _hasClientMaxBodySize(false),
		_hasReturnDirective(false)
	{}
	// Location (const Location& other):
	// 	_uri(other._uri),
	// 	_autoindex(other._autoindex),
	// 	_hasAutoindex(other._hasAutoindex),
	// 	_clientMaxBodySize(other._clientMaxBodySize),
	// 	_hasClientMaxBodySize(other._hasClientMaxBodySize),
	// 	_returnDirective(other._returnDirective),
	// 	_hasReturnDirective(other._hasReturnDirective),
	// 	_cgiExec(other._cgiExec),
	// 	_cgiParams(other._cgiParams),
	// 	_limitExceptMethods(other._limitExceptMethods),
	// 	_root(other._root),
	// 	_index(other._index)
	// {}

	inline std::string				getUri() const				{ return _uri; }
	inline bool						getAutoindex() const		{ return _autoindex; }

	inline std::string				getCgiExec() const		{ return _cgiExec; }
	inline size_t					getClientMaxBodySize() const{ return _clientMaxBodySize; }
	inline const ReturnDirective&	getReturnDirective() const	{ return _returnDirective; }
	inline std::string				getRoot() const				{ return _root; }
	inline std::string				getIndex() const			{ return _index; }

	inline bool						hasAutoindex() const		{ return _hasAutoindex; }
	inline bool						hasClientMaxBodySize() const{ return _hasClientMaxBodySize; }
	inline bool						hasReturnDirective() const	{ return _hasReturnDirective; }

	void 	setAutoindex(bool autoindex) {
		 _autoindex = autoindex;
		 _hasAutoindex = true;
	}
	void 	setClientMaxBodySize(size_t size)	{
		_clientMaxBodySize = size;
		_hasClientMaxBodySize = true;
	}
	void 	setReturnDirective(const ReturnDirective& returnDirective)	{
		_returnDirective = returnDirective;
		_hasReturnDirective = true;
	}
	void 	setUri(std::string uri)				{ _uri = uri; }
	void 	setCgiExec(const std::string& exec)	{ _cgiExec = exec;}
	void 	setRoot(std::string root)			{_root = root;}
	void 	setIndex(std::string index)			{_index = index;}

	void	addCgiParam(const std::string& name, const std::string& value) { _cgiParams[name] = value;}
	void	addCgiParam(const std::pair<std::string,std::string> pair) 	{ _cgiParams[pair.first] = pair.second;}

	/// return the map of cgiParam (usefull for testing purpose)
	inline std::map<std::string, std::string>&	getCgiParams() 	{ return _cgiParams;}
	inline std::string	getCgiParam(std::string param) 			{ return _cgiParams[param];}

	void	addLimitExceptMethod(const std::string& method)			{_limitExceptMethods.insert(method);}
	void	addLimitExceptMethods(const std::set<std::string>& l)	{ _limitExceptMethods.insert(l.begin(), l.end());}
	inline	std::set<std::string>&	getLimitExceptMethods() 		{ return _limitExceptMethods;}
	bool	hasLimitExceptMethods(const std::string& method)		{ return _limitExceptMethods.count(method);}

private:
	std::string							_uri;
	
	bool								_autoindex;
	bool								_hasAutoindex;
	
	size_t								_clientMaxBodySize;
	bool								_hasClientMaxBodySize;

	ReturnDirective						_returnDirective;
	bool								_hasReturnDirective;

	std::string							_cgiExec;
	std::map<std::string, std::string> 	_cgiParams;

	std::set<std::string>			_limitExceptMethods;

	std::string							_root;
	std::string							_index;
}; /* class Location */

class ServerBlock
{
public:
	ServerBlock(): _autoindex(false), _hasAutoindex(false),
		_clientMaxBodySize(DEFAULT_CLIENT_MAX_BODY_SIZE), _hasClientMaxBodySize(false),
		_hasReturnDirective(false) {}

	inline std::string				getIndex() const			{ return _index; }
	inline bool						getAutoindex() const		{ return _autoindex; }
	inline std::string				getRoot() const				{ return _root; }
	inline std::string				getServerName() const		{ return _serverName; }
	inline size_t					getClientMaxBodySize() const{ return _clientMaxBodySize; }
	inline const ReturnDirective&	getReturnDirective() const	{ return _returnDirective; }

	inline bool						hasAutoindex() const		{ return _hasAutoindex; }
	inline bool						hasClientMaxBodySize() const{ return _hasClientMaxBodySize; }
	inline bool						hasReturnDirective() const	{ return _hasReturnDirective; }

	void 	setAutoindex(bool autoindex) {
		 _autoindex = autoindex;
		 _hasAutoindex = true;
	}
	void 	setClientMaxBodySize(size_t size) {
		_clientMaxBodySize = size;
		_hasClientMaxBodySize = true;
	}
	void 	setReturnDirective(const ReturnDirective& returnDirective)	{
		_returnDirective = returnDirective;
		_hasReturnDirective = true;
	}

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
	
	size_t								_clientMaxBodySize;
	bool								_hasClientMaxBodySize;
	
	ReturnDirective						_returnDirective;
	bool								_hasReturnDirective;

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
	/// ONLY FOR TESTING PURPOSE
	static void	__delete_singleton_instance();

	inline std::string		getConfigFilePath() const { return _configFilePath;}

	/// return the vector of servers (usefull for testing)
	inline const std::vector<ServerBlock>&	getServers() const			{ return _servers;}
	/// return server from given index (usefull for testing purpose)
	inline ServerBlock&				getServer(uint32_t index) 	{ return _servers[index];}
	/// return the server who own the given port
	//ServerBlock&					findServer(uint32_t port);

	ServerBlock&					findServer(const Uri& uri);

	/// return all ports defined in listen directives.
	std::vector<uint32_t>			getPorts();

private:
	ServerConfig(const std::string & filepath);
	ServerConfig(ServerConfig&);
	ServerConfig& operator=(const ServerConfig&);
	
	void								_parse(std::istream &);

	ServerBlock							_parseServer(parser::config::ScannerConfig & scanner, parser::config::Token serverToken);
	Host								_parseListen(parser::config::ScannerConfig & scanner);
	std::string 						_parseRoot(parser::config::ScannerConfig & scanner);
	std::string 						_parseIndex(parser::config::ScannerConfig & scanner);
	bool								_parseAutoindex(parser::config::ScannerConfig & scanner);
	std::string 						_parseServerName(parser::config::ScannerConfig & scanner);
	std::map<u_short, std::string>		_parseErrorPage(parser::config::ScannerConfig & scanner);
	Location							_parseLocation(parser::config::ScannerConfig & scanner, parser::config::Token locationToken);
	Host 								_parseHost(parser::config::ScannerConfig & scanner);
	std::string							_parseCgiExec(parser::config::ScannerConfig & scanner);
	std::pair<std::string, std::string>	_parseCgiParam(parser::config::ScannerConfig & scanner);
	size_t								_parseClientMaxBodySize(parser::config::ScannerConfig & scanner);
	ReturnDirective						_parseReturn(parser::config::ScannerConfig & scanner);
	std::set<std::string>				_parseLimitExceptMethods(parser::config::ScannerConfig & scanner);
	
	Host _parseListenValue(const parser::config::Token& host);

	void _postParser();
	void _postParserSetAutoindexInChilds();
	void _postParserSetClientMaxBodySizeInChilds();
	void _postParserSet();

	void _skipSemiColonNewLine(parser::config::ScannerConfig & scanner);
	void _throw_SyntaxError(parser::config::Token t, const std::string &error_str);

	static ServerConfig*		_singleton;
	std::vector<ServerBlock>	_servers;
	std::string					_configFilePath;

}; /* class ServerConfig */

#endif /* SERVER_CONFIG_HPP */
