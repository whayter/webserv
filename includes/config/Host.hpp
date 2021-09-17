#ifndef HOST_HPP
#define HOST_HPP

#include <string>

class Host
{
public:
	Host() :  _port(0) {} 
	Host(std::string hostname, uint32_t port) : _hostname(hostname), _port(port) {} 
	Host(uint32_t port) : _port(port) {} 
	Host(std::string hostname) : _hostname(hostname), _port(0) {} 

	inline std::string	getHostname() const	{ return _hostname; }
	inline uint32_t		getPort() const		{ return _port; }

	void setHostname(std::string hostname)	{ _hostname = hostname; }
	void setPort(uint32_t port) 			{ _port = port; }

private:
	std::string _hostname;
	uint32_t	_port;
}; /* class Host */

#endif /* HOST_HPP */
