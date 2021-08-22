/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 17:50:07 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/22 11:33:59 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERHANDLER_HPP
# define SERVERHANDLER_HPP

# include "Socket.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"

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
			void 				stop(int status = 0);

		private:

		/* --- Private functions -------------------------------------------- */

			void				_connectClients(int serverSocket);
			void				_disconnectClient(int index);
			void				_serveClient(int index);
			
			bool				_getRequest(int index);
			void				_processRequest(int index);
			void				_sendResponse(int index);

			bool				_isServerSocket(int index);
			void				_updateData();
			void				_cout(int index, std::string message);

		/* --- Disabled functions ------------------------------------------- */

			ServerHandler(const ServerHandler&);
		
		/* --- Member variables --------------------------------------------- */

			std::vector<Server> _servers;
			struct pollfd		_fds[SOMAXCONN];
			HttpRequest*		_requests[SOMAXCONN];
			int					_nfds;
			int					_firstClientIndex;
			bool				_upToDateData;
			bool 				_disconnect;
	};
};

#endif