/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 15:01:14 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/13 15:10:58 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEREVER_CONFIG_HPP
#define SEREVER_CONFIG_HPP

#include <vector>
#include <string>


namespace config
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
	
	struct Server
	{
		std::vector<listen>  listens;		
	};
	

	std::vector<>	
};

}

#endif