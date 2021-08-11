/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/08 12:46:58 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/11 12:26:07 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Socket.hpp"

# include <poll.h>

namespace clnt
{
	/* --- class Client ----------------------------------------------------- */

	class Client
	{
		public:

		/* --- Member functions --------------------------------------------- */


			Client();
			~Client();

			inline sckt::fd_type	getFd() const { return _socket.getFd(); }
			inline sckt::addr_type	getAddr() const { return _socket.getAddr(); }
			inline struct pollfd	getPfd() const { return _pfd; }

			void					setFd(sckt::fd_type fd);
			void					setAddr(sckt::addr_type addr);
			void					setPfd(sckt::fd_type fd);

		private:

		/* --- Member variables --------------------------------------------- */

			sckt::Socket			_socket;
			struct pollfd			_pfd;
			
		/* --- Disabled default functions ----------------------------------- */

			// Client(const Client &);
			// Client& operator=(const Client&);
	};
	
	/* --- Namespace functions ---------------------------------------------- */

	bool	connect(sckt::fd_type fd, sckt::addr_type addr);
	void	disconnect(const Client& client);
};

#endif