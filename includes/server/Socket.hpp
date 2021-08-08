/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/08 12:23:12 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/08 23:38:59 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <cstdlib>
# include <cstring>
# include <iostream>
# include <netinet/in.h>
# include <fcntl.h>
# include <unistd.h>

namespace sckt
{
	/* --- types ---------------------------------------------------------------- */

		typedef int					fd_type;
		typedef struct sockaddr_in	addr_type;
		typedef socklen_t			addrLen_type;

	/* --- class Socket --------------------------------------------------------- */

	class Socket
	{
		public:

			Socket() {}
			~Socket() {}

			inline fd_type			getFd() { return _fd; }
			inline addr_type		getAddr() { return _addr; }
			
			void					setFd(int domain, int type)
			{
				if ((_fd = socket(domain, type, 0)) < 0)
				{
					std::cerr << "Error: socket()" << std::endl;
					exit(EXIT_FAILURE);
				}
			}		
			void					setFd(fd_type fd)
			{
				_fd = fd;
			}
			
			void					setAddr(int domain, int interface, int port)
			{
				_addr.sin_family = domain;
				_addr.sin_addr.s_addr = interface;
				_addr.sin_port = htons(port);
				memset(_addr.sin_zero, 0, sizeof(_addr.sin_zero));
				_addrLen = static_cast<socklen_t>(sizeof(_addr));
			}
			void					setAddr(addr_type addr)
			{
				_addr = addr;
			}

		private:

			fd_type					_fd;
			addr_type				_addr;
			addrLen_type			_addrLen;
	};

	/* --- functions ------------------------------------------------------------ */

	void closeSocket(fd_type fd)
	{
		if (fd <= 0)
			return ;
		if (close(fd) < 0)
		{
			std::cerr << "Error: close()" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	void setNonBlocking(fd_type fd)
	{
		if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		{
			closeSocket(fd);
			std::cerr << "Error: fcntl()" << std::endl;
			//exit(EXIT_FAILURE);
		}
	}

	bool setReusableAddr(fd_type fd)
	{
		int opt = 1;
		return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == 0;
	}

	void bindSocket(fd_type fd, addr_type addr)
	{
		if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
		{
			closeSocket(fd);
			std::cerr << "Error: bind()" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	void listenSocket(fd_type fd)
	{
		if(listen(fd, SOMAXCONN) < 0)
		{
			closeSocket(fd);
			std::cerr << "Error: listen()" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
};

#endif