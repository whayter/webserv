/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/08 12:39:38 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/08 23:40:09 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Socket.hpp"
# include "Client.hpp"

# include <vector>

class Server
{
	public:

	/* --- Member types ----------------------------------------------------- */

		typedef std::vector<client::Client>		clients_type;

	/* --- Member functions ------------------------------------------------- */

		Server(int port)
		{
			_socket.setFd(AF_INET, SOCK_STREAM);
			_socket.setAddr(AF_INET, INADDR_ANY, port);
			sckt::setNonBlocking(_socket.getFd());
			sckt::bindSocket(_socket.getFd(), _socket.getAddr());
			sckt::listenSocket(_socket.getFd());
		}
		
		~Server()
		{
			sckt::closeSocket(_socket.getFd());
		}

		sckt::fd_type allowLink(sckt::addr_type addr, sckt::addrLen_type len)
		{
			sckt::fd_type fd = _socket.getFd();
			sckt::fd_type newClientFd;
			newClientFd = accept(fd, (struct sockaddr *)&addr, &len);
			return newClientFd;
		}
	
		// int getRequest()
		// {
			
		// }
		


		clients_type getClients()
		{
			return _clients;
		}

		void addNewClient(client::Client client)
		{
			_clients.push_back(client);
		}

		void removeClient(clients_type::iterator client)
		{
			sckt::closeSocket(client->getFd());
			_clients.erase(client);
		}
		
	private:

	/* --- Member variables ------------------------------------------------- */

		sckt::Socket			_socket;
		clients_type			_clients;
};

#endif