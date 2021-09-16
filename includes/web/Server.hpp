/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 23:15:15 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/16 16:06:06 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "web.hpp"
#include "Device.hpp"

#include <vector>
#include <poll.h>

namespace web {

class Server {

public:

	Server();
	~Server();

	bool					setup();
	void					routine();
	void					stop(int status = 0);

private:

	/* --- Private functions ------------------------------------------------ */

	void					_connectClients(int servrIndex);
	void					_disconnectDevice(int deviceIndex);

	void					_getRequests(int deviceIndex);
	void					_buildRequests(int deviceIndex);
	void					_buildResponses(int deviceIndex);
	void					_sendResponses(int deviceIndex);

	bool					_isServerIndex(int deviceIndex);
	void					_log(int fd, std::string description);

	/* --- Muted functions -------------------------------------------------- */

	Server(const Server& ref);

	/* --- Member variables ------------------------------------------------- */

	std::vector<Device> 	_devices;
	struct pollfd			_fds[SOMAXCONN];
	int						_nfds;
	int						_firstClientIndex;

}; /* class Server */

}; /* namespace web */

#endif /* SERVER_HPP */