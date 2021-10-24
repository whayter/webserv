#ifndef HOST_HPP
#define HOST_HPP

#include <string>
#include <stdint.h>

class Host
{
public:
	Host() :  _port(0) {} 
	Host(std::string hostname, uint16_t port) : _hostname(hostname), _port(port) {} 
	Host(uint32_t port) : _port(port) {} 
	Host(std::string hostname) : _hostname(hostname), _port(0) {} 

	std::string	getHostname() const;
	uint16_t	getPort() const;

	void setHostname(std::string hostname);
	void setPort(uint16_t port);

private:
	std::string _hostname;
	uint16_t	_port;
}; /* class Host */

inline std::string	Host::getHostname() const	{ return _hostname; }
inline uint16_t		Host::getPort() const		{ return _port; }

#endif /* HOST_HPP */
