#ifndef WEB_HPP
#define WEB_HPP

#include <netinet/in.h>

namespace web {

/* --- Types ---------------------------------------------------------------- */

typedef int					fd_type;
typedef struct sockaddr_in	addr_type;
typedef socklen_t			addrLen_type;

/* --- Classes -------------------------------------------------------------- */

class 						Socket;
class						Device;
class 						Server;

}; /* namespace web */

#endif /* WEB_HPP */