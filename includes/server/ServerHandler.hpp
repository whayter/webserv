/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 17:50:07 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/30 10:00:22 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERHANDLER_HPP
# define SERVERHANDLER_HPP

# include "Socket.hpp"
# include "HttpRequest.hpp"
#include "ServerConfig.hpp"

# include <poll.h>
# include <vector>

namespace server
{
	struct Server
	{
		ServerBlock				block;
		sckt::Socket			socket;
		uint32_t				port;
		int						index;
	};

	class ServerHandler
	{
		public:

		/* --- Member functions --------------------------------------------- */

			ServerHandler();
			~ServerHandler();

			bool 				start(void);
			void 				run(void);
			void 				stop(int status = 0);

		private:

		/* --- Private functions -------------------------------------------- */

			void				_connectClients(int serverSocket);
			void				_disconnectClient(int index);
			void				_serveClient(int index);
			
			void				_getRequest(int index);
			void				_sendResponse(int index, const char* response);

			bool				_isServerSocket(int index);
			void				_updateData(void);
			void				_log(int fd, std::string message);

		/* --- Disabled functions ------------------------------------------- */

			ServerHandler(const ServerHandler&);
		
		/* --- Member variables --------------------------------------------- */

			std::vector<Server> _servers;
			struct pollfd		_fds[SOMAXCONN];
			HttpRequest*		_requests[SOMAXCONN];
			int					_nfds;
			int					_firstClientIndex;
			bool				_upToDateData;
	};
};

#endif