#include "Context.hpp"

Context getContext(const Uri& uri)
{
	ServerConfig& config = ServerConfig::getInstance();
	ServerBlock& server = config.findServer(uri);
	fs::path path = server.getPathFromUri(uri);
	const Location& location = server.findLocation(path.c_str());	// with this line uncommented, simple calls to cgi work, but return directives don't work
	// const Location& location = server.findLocation(uri);		        // with this line uncommented, return directives work, but simple calls to cgi don't
	return Context(server, location, path, uri);
}