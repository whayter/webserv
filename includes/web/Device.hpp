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

	void					setPort(port_type port);

	inline port_type		getPort() { return _port; }
	inline Socket&			getSocket() { return _socket; }
	buffer_type&			getInputBuffer() { return _inputBuffer; }
	buffer_type&			getOutputBuffer() { return _outputBuffer; }

	requests_queue_type&	getRequestsQueue() { return _requests; }
	responses_queue_type&	getResponsesQueue() { return _responses; }

	void					closeSocket();
	bool					setNonBlocking();
	bool					setReusableAddr();
	bool					bindSocket();
	bool					listenSocket();

private:

	port_type				_port;
	Socket					_socket;
	buffer_type				_inputBuffer;
	buffer_type				_outputBuffer;
	requests_queue_type		_requests;
	responses_queue_type	_responses;
};

} /* namespace web */

#endif /* DEVICE_HPP */