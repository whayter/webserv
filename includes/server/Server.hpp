/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/08 12:39:38 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/14 16:36:20 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Socket.hpp"
# include "Client.hpp"

# include <vector>
# include <poll.h>

class Server
{
	public:

	/* --- Member types ----------------------------------------------------- */

		typedef std::vector<clnt::Client>	clients_type;

	/* --- Member functions ------------------------------------------------- */

		Server(int port);
		Server(const Server& s);
		Server& operator=(const Server& s);
		~Server();

		bool				start();
		void				update();
		void				stop();

		void 				connectClient();		
		void				manageClient(clients_type::iterator client);

		bool				getRequest(sckt::fd_type fd, std::string* request);
		bool getReq(sckt::fd_type fd);

		void 				disconnectClient(clients_type::iterator client);

	private:

	/* --- Member variables ------------------------------------------------- */

		int					_port;
		sckt::Socket		_socket;
		clients_type		_clients;
		struct pollfd		_pfd;
};

#endif