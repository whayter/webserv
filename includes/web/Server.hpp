#ifndef SERVER_HPP
#define SERVER_HPP

#include "web.hpp"
#include "Device.hpp"

#include <vector>
#include <poll.h>

namespace web {

class Server {

	/* --- Member functions -------------------------------------------------- */

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