#ifndef WEB_HPP
#define WEB_HPP

#include <netinet/in.h>
#include <queue>
#include <deque>

# include "http.hpp"

namespace web {

/* --- Types ---------------------------------------------------------------- */

typedef uint32_t								port_type;
typedef int										fd_type;
typedef struct sockaddr_in						addr_type;
typedef socklen_t								addrLen_type;
typedef std::vector<unsigned char>				buffer_type;
typedef std::queue<http::Request>				requests_queue_type;
typedef std::queue<http::Response>				responses_queue_type;

/* --- Classes -------------------------------------------------------------- */

class 											Socket;
class											Device;
class 											Server;

}; /* namespace web */

#endif /* WEB_HPP */