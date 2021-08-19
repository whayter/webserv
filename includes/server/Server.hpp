/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 17:50:07 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/16 15:33:45 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Socket.hpp"
#include "http/HttpRequest.hpp"

# include <poll.h>

# define BACKLOG 32

class Server
{
	public:

	/* --- Member functions ------------------------------------------------- */

		Server(int port);
		~Server();

		bool start();
		void update();
		void stop();

	private:

	/* --- Private functions ------------------------------------------------ */

		void _manageClient(int index);
		bool _getRequest(int fd);
		bool _connectClient();
		void _disconnectClient(int index);
		void _updateFds();

	/* --- Member variables ------------------------------------------------- */

		int					_port;
		sckt::Socket		_socket;
		struct pollfd		_fds[BACKLOG];
		int					_nfds;
		bool				_upToDateFds;
};

#endif