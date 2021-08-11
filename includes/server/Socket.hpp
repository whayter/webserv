/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/08 12:23:12 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/11 12:27:01 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

//# include <cstdlib>
# include <cstring>
# include <iostream>
# include <netinet/in.h>
# include <fcntl.h>
# include <unistd.h>

# define INVALID_FD -1

namespace sckt
{
	/* --- Types ------------------------------------------------------------ */

	typedef int					fd_type;
	typedef struct sockaddr_in	addr_type;
	typedef socklen_t			addrLen_type;

	/* --- Class Socket ----------------------------------------------------- */

	class Socket
	{
		public:

		/* --- Member functions --------------------------------------------- */

			Socket();
			~Socket();

			inline fd_type		getFd() const { return _fd; }
			inline addr_type	getAddr() const { return _addr; }
			inline addrLen_type	getAddrLen() const { return _addrLen; }
			
			bool				setFd(int domain, int type);
			void				setFd(fd_type fd);
			void				setAddr(int domain, int interface, int port);
			void				setAddr(const addr_type& addr);

		private:

		/* --- Member variables --------------------------------------------- */

			fd_type				_fd;
			addr_type			_addr;
			addrLen_type		_addrLen;

		/* --- Disabled default functions ----------------------------------- */

			// Socket(const Socket&);
			// Socket&	operator=(const Socket&);
	};

	/* --- Namespace functions ---------------------------------------------- */

	void						closeSocket(fd_type fd);
	bool						setNonBlocking(fd_type fd);
	bool						setReusableAddr(fd_type fd);
	bool						bindSocket(fd_type fd, const addr_type& addr);
	bool						listenSocket(fd_type fd);
};

#endif