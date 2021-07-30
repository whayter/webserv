/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/30 12:32:12 by hwinston          #+#    #+#             */
/*   Updated: 2021/07/30 17:56:40 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"

# define PORT 8080												// temporary
# define BUF_SIZE 30000											// temporary

# define BACKLOG 10

class server
{    
	public:

	/* --- Member types ----------------------------------------------------- */

		typedef long					fd_type;
		typedef long					socket_type;
		typedef struct sockaddr_in		addr_type;

	/* --- Member functions ------------------------------------------------- */

		server(void) {}
		
		~server(void) {}

		int	init(void)
		{
			if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			{
				std::cerr << "Error: socket" << std::endl;
				return 0;
			}
			_addr.sin_family = AF_INET;
    		_addr.sin_addr.s_addr = INADDR_ANY;
    		_addr.sin_port = htons(PORT);
			memset(_addr.sin_zero, 0, sizeof _addr.sin_zero);
			if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
			{
				std::cerr << "Error: bind" << std::endl;
				return 0;
			}
			if (listen(_fd, BACKLOG) < 0)
			{
				std::cerr << "Error: listen" << std::endl;
				return 0;
			}
			return 1;
		}

		socket_type accept_connection(void)
		{
			socket_type new_socket;
			socklen_t addr_len = static_cast<socklen_t>(sizeof(_addr));
			
			if ((new_socket = accept(_fd, (struct sockaddr *)&_addr, &addr_len)) < 0)
			{
				std::cerr << "Error: accept" << std::endl;
				return 0;
			}
			return new_socket;
		}

		int read_from_socket(socket_type socket)
		{
			char buffer[BUF_SIZE] = {0};
			int valread;

			if ((valread = recv(socket, buffer, BUF_SIZE - 1, 0)) == -1)
			{
				std::cerr << "Error: recv" << std::endl;
				return 0;
			}
			std::cout << buffer << std::endl;
			return 1;
		}

		void close_socket(socket_type socket)
		{
			if (socket > 0)
				close(socket);
		}

		void say_hello(socket_type socket)							// temporary
		{
			const char* hello = "Hello from server";
			write(socket, hello, strlen(hello));
		}
		
	private:

	/* --- Member variables ------------------------------------------------- */

		fd_type							_fd;
		addr_type						_addr;
};

#endif