/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 15:53:38 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/11 16:54:04 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERHANDLER_HPP
# define SERVERHANDLER_HPP

# include "Server.hpp"


/*
** This file is not completed. We will probably need to add some things,
** according to what will be asked in the configuration file.
**
** Constructor param is temporary.
*/

class ServerHandler
{
	public:

		ServerHandler(std::vector<int> ports);
		~ServerHandler();

		void run();

	private:

		std::vector<Server>	_servers;
};

#endif