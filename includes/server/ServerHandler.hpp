/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 17:50:07 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/20 16:41:41 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERHANDLER_HPP
# define SERVERHANDLER_HPP

# include "Socket.hpp"
# include "HttpRequest.hpp"

# include <poll.h>
# include <vector>

namespace server
{
	struct Server
	{
		sckt::Socket			socket;
		uint32_t				port;
		int						index;
	};

	class ServerHandler
	{
		public:

		/* --- Member functions --------------------------------------------- */

			ServerHandler(std::vector<uint32_t> ports);
			~ServerHandler();

			bool 				start();
			void 				update();
			void 				stop(int status);

		private:

		/* --- Private functions -------------------------------------------- */

			void				_serveClient(int index);
			void				_connectClients(int serverSocket);
			void				_disconnectClient(int index);
			bool				_getRequest(int fd);		

			bool				_isServerSocket(int index);
			void				_updateFds();
		
		/* --- Member variables --------------------------------------------- */

			std::vector<Server> _servers;
			struct pollfd		_fds[SOMAXCONN];
			int					_nfds;
			int					_firstClientIndex;
			bool				_upToDateFds;
	};
};

#endif