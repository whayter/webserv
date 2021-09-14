/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 16:20:05 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/13 14:42:13 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "web.hpp"
#include <string>

#define INVALID_FD -1

namespace web {

class Socket {

public:

	Socket();
	~Socket();

	inline fd_type		getFd() const { return _fd; }
	inline addr_type	getAddr() const { return _addr; }
	inline addrLen_type	getAddrLen() const { return _addrLen; }

	std::string			getIp();
	
	bool				setFd(int domain, int type);
	void				setFd(fd_type fd);
	void				setAddr(int domain, int interface, int port);
	void				setAddr(const addr_type& addr);

private:

	fd_type				_fd;
	addr_type			_addr;
	addrLen_type		_addrLen;

}; /* class Socket */

}; /* namespace web */

#endif /* SOCKET_HPP */