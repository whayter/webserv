#ifndef HTTP_HPP
#define HTTP_HPP

#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <map>

namespace http
{
   	/* --- Types ------------------------------------------------------------ */

    typedef std::queue<unsigned char>			queue_type;
	typedef std::map<std::string, std::string>	headers_type;
	typedef std::vector<unsigned char>			content_type;
		
	/* --- Classes & structures --------------------------------------------- */

	class										Message;
	class										Request;
	class										Response;
	struct										Status;
};

#endif