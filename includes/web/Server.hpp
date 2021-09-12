/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 23:15:15 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/11 10:15:18 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "web.hpp"
#include "Device.hpp"
#include "HttpRequest.hpp"

#include <vector>
#include <poll.h>

class web::Server
{
	public:

	/* --- Member functions ------------------------------------------------- */

		Server();
		~Server();

		bool					setup();
		void					routine();
		void					stop(int status = 0);

	private:

	/* --- Private functions ------------------------------------------------ */

		void					_connectClients(int serverFd);
		void					_disconnectDevice(int deviceIndex);
		void					_serveClient(int index);

		void					_getRequest(int index);
		void					_sendResponse(int index, const char* response);

		bool					_isServerIndex(int index);
		void					_log(int fd, std::string description);

	/* --- Disabeld functions ----------------------------------------------- */

		Server(const Server& ref);

	/* --- Member variables ------------------------------------------------- */

		std::vector<Device> 	_devices;
		struct pollfd			_fds[SOMAXCONN];
		int						_nfds;
		int						_firstClientIndex;
		HttpRequest*			_requests[SOMAXCONN];
};

#endif