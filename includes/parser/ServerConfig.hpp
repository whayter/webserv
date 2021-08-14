/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 15:01:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/13 17:02:04 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEREVER_CONFIG_HPP
#define SEREVER_CONFIG_HPP

#include <vector>
#include <map>
#include <string>
#include <cctype>

#include "Uri.hpp"

// Singleton
class ServerConfig
{
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


	ServerConfig& getInstance(std::string filepath);
	
	inline std::string getFilepath() const { return _filepath;}

private:
	ServerConfig(std::string filepath);
	ServerConfig(ServerConfig&);
	ServerConfig& operator=(const ServerConfig&);
	

	static ServerConfig* _singleton;

	std::vector<ServerBlock> _servers;
	std::string _filepath;
};

#endif /* SERVER_CONFIG_HPP */
