/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Device.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/08 16:56:40 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/15 14:51:34 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "web.hpp"
#include "Socket.hpp"
#include "Request.hpp"
#include "Response.hpp"

#include <queue>
#include <deque>

namespace web {

class Device {

public:

	Device() {}
	~Device() {}

	void								setPort(uint32_t port);

	inline uint32_t						getPort() { return _port; }
	inline Socket&						getSocket() { return _socket; }
	inline std::deque<unsigned char>	getBuffer() { return _buffer; }

	void								closeSocket();
	bool								setNonBlocking();
	bool								setReusableAddr();
	bool								bindSocket();
	bool								listenSocket();

private:

	uint32_t							_port;
	Socket								_socket;
	std::deque<unsigned char>			_buffer;
	std::queue<http::Request>			_requests;
	std::queue<http::Response>			_responses;
};

} /* namespace web */

#endif /* DEVICE_HPP */