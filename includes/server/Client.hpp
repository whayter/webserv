/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/08 12:46:58 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/08 21:59:51 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Socket.hpp"

namespace client
{
	/* --- class Client ----------------------------------------------------- */

	class Client
	{
		public:

			Client() {}
			~Client() {}

			inline sckt::fd_type	getFd() { return _socket.getFd(); }
			inline sckt::addr_type	getAddr() { return _socket.getAddr(); }

			void					setFd(sckt::fd_type fd)
			{
				_socket.setFd(fd);
			}			
			void					setAddr(sckt::addr_type addr)
			{
				_socket.setAddr(addr);
			}

		private:

			sckt::Socket	_socket;
	};
	
	/* --- functions -------------------------------------------------------- */

	void connectClient(sckt::fd_type fd, sckt::addr_type addr)
	{
		if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		{
			close(fd);
			std::cerr << "Error: connect()" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
};

#endif