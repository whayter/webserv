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

#include "parser/ServerConfig.hpp"

ServerConfig* ServerConfig::_singleton = nullptr;

ServerConfig::ServerConfig(std::string filepath)
	: _filepath(filepath)
{

}

ServerConfig& ServerConfig::getInstance(std::string filepath){
	if (_singleton == NULL)
		_singleton == new ServerConfig(filepath);
	return *_singleton;
}
