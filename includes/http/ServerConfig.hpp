/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 15:01:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/13 15:13:31 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEREVER_CONFIG_HPP
#define SEREVER_CONFIG_HPP

#include <vector>
#include <string>


namespace config
{
	
	struct Server
	{
		struct Listen
		{
			std::string host;
			int port;
		};
		
		struct Location
		{
			bool autoindex;
		};
		
		std::vector<Listen> 	listens;
		std::string				root;		
		std::string				serverName;		
	};
	

	std::vector<Server> servers;	
};

}

#endif