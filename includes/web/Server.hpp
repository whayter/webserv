/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 23:15:15 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/13 11:21:37 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "web.hpp"
#include "Device.hpp"
#include "HttpRequest.hpp"

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
	void					_serveClient(int deviceIndex);

	void					_getRequest(int deviceIndex);
	void					_sendResponse(int deviceIndex, const char* response);

	bool					_isServerIndex(int deviceIndex);
	void					_log(int fd, std::string description);

	/* --- Muted functions -------------------------------------------------- */

	Server(const Server& ref);

	/* --- Member variables ------------------------------------------------- */

	std::vector<Device> 	_devices;
	struct pollfd			_fds[SOMAXCONN];
	int						_nfds;
	int						_firstClientIndex;
	HttpRequest*			_requests[SOMAXCONN];

}; /* class Server */

}; /* namespace web */

#endif /* SERVER_HPP */