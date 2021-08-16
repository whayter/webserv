/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 15:01:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/16 15:59:58 by juligonz         ###   ########.fr       */
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
class ServerConfig
{
public:
	struct ServerBlock
	{
		struct Host
		{
			std::string host;
			int			port;
		};

		struct Location
		{
			Uri	path;

			/// Syntax:	 autoindex on | off;
			/// Default: autoindex off;
			/// Context: http, server, location
			/// http://nginx.org/en/docs/http/ngx_http_autoindex_module.html
			bool	autoindex;
			Host	fastCgiPass;
			///	Syntax:	client_max_body_size size;
			/// Default: client_max_body_size 1m;
			/// Context:	http, server, location
			/// http://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size
			size_t	client_max_body_size;
			// std::map<std::string, std::string> 	fatsCgiParam;
		};

		std::vector<Host> 					listens;
		std::string							serverName;
		std::string							root;	
		std::vector<std::string> 			indexes;
		std::map<u_short, std::string> 		errors;

	};


	static ServerConfig* getInstance(std::string filepath);
	

	inline std::string getConfigFilePath() const { return _configFilePath;}


private:
	ServerConfig(const std::string & filepath);
	ServerConfig(ServerConfig&);
	ServerConfig& operator=(const ServerConfig&);
	
	void _parse(std::istream &);
	void _parseServer(parser::config::ScannerConfig &);

	void _thow_SyntaxError(parser::config::Token t, const std::string &error_str);

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
