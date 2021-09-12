/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Device.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 16:56:40 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/11 10:18:36 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "web.hpp"
#include "Socket.hpp"

//#define BUFFER_SIZE 4096

class web::Device
{
	public:

	/* --- Member functions ------------------------------------------------- */

		Device() {}
		~Device() {}

		void			setPort(uint32_t port);

		inline uint32_t	getPort() { return _port; }
		inline Socket&	getSocket() { return _socket; }
		//inline char*	getBuffer() { return _buffer; }

	private:

	/* --- Member variables ------------------------------------------------- */

		uint32_t		_port;
		Socket			_socket;
		//char			_buffer[BUFFER_SIZE];
};

#endif