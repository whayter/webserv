/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 15:01:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/13 16:16:47 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEREVER_CONFIG_HPP
#define SEREVER_CONFIG_HPP

#include <vector>
#include <map>
#include <string>
#include <cctype>


namespace config
{
struct Server
{
	struct Host
	{
		std::string host;
		int			port;
	};

	struct Location
	{
		bool	autoindex;
		Host	fastCgiPass;
		
	};

	std::vector<Host> 					listens;
	std::string							serverName;
	std::string							root;		
	std::vector<std::string> 			indexes;
	std::vector<u_short, std::string> 	errors;
};

std::vector<Server> servers;	


}; /* namespace config */

#endif