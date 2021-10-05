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
	inline std::vector<unsigned char>&	getInputBuffer() { return _inputBuffer; }
	inline std::vector<unsigned char>&	getOutputBuffer() { return _outputBuffer; }

	inline std::queue<http::Request>&	getRequestsQueue() { return _requests; }
	inline std::queue<http::Response>&	getResponsesQueue() { return _responses; }

	void								closeSocket();
	bool								setNonBlocking();
	bool								setReusableAddr();
	bool								bindSocket();
	bool								listenSocket();

private:

	uint32_t							_port;
	Socket								_socket;
	std::vector<unsigned char>			_inputBuffer;
	std::vector<unsigned char>			_outputBuffer;
	std::queue<http::Request>			_requests;
	std::queue<http::Response>			_responses;
};

} /* namespace web */

#endif /* DEVICE_HPP */