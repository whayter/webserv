/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 15:01:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/16 20:00:29 by juligonz         ###   ########.fr       */
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
	
struct ServerBlock
{
	struct Host
	{
		std::string host;
		uint32_t	port;
	};
	struct Location
	{
		std::string						uri;
		bool							autoindex;
		Host							fastCgiPass;
		size_t								client_max_body_size;
		std::map<std::string, std::string> 	fatsCgiParam;
	};

	std::vector<Host>					listens;
	std::string							serverName;
	std::string							root;	
	std::vector<std::string> 			indexes;
	std::map<u_short, std::string> 		errors;
};

class ServerConfig
{
public:



	static ServerConfig& getInstance(std::string filepath);
	inline std::string getConfigFilePath() const { return _configFilePath;}


	inline std::vector<ServerBlock>	getServers()				{ return _servers;}
	inline ServerBlock&				getServer(uint32_t index)	{ return _servers[index];}
	ServerBlock&					findServer(uint32_t port);


private:
	ServerConfig(const std::string & filepath);
	ServerConfig(ServerConfig&);
	ServerConfig& operator=(const ServerConfig&);
	
	void _parse(std::istream &);

	ServerBlock			_parseServer(parser::config::ScannerConfig & scanner);
	ServerBlock::Host	_parseListen(parser::config::ScannerConfig & scanner);
	std::string _parseRoot(parser::config::ScannerConfig & scanner);
	std::string _parseIndex(parser::config::ScannerConfig & scanner);
	void _parseServerName(parser::config::ScannerConfig & scanner);
	void _parseErrorPage(parser::config::ScannerConfig & scanner);
	void _parseLocation(parser::config::ScannerConfig & scanner);
	
	ServerBlock::Host _parseListenValue(const parser::config::Token& host);
	ServerBlock::Host _parseHost(const parser::config::Token& host);

	void _skipSemiColonNewLine(parser::config::ScannerConfig & scanner);

	void _throw_SyntaxError(parser::config::Token t, const std::string &error_str);

	static ServerConfig* _singleton;
	std::vector<ServerBlock> _servers;
	std::string _configFilePath;

public:

	class SyntaxError: public std::exception
	{
		public:
			SyntaxError(const char * message)
				: _message(message) {}
			SyntaxError(const std::string& message)
				: _message(message) {}
			virtual ~SyntaxError() {}

			virtual const char* what() const throw() {
				return _message.c_str();
			}

		protected:
			std::string _message;
	};

}; /* class ServerConfig */

#endif /* SERVER_CONFIG_HPP */
