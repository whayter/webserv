#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "ServerConfig.hpp"

namespace fs = ft::filesystem;

struct Context {

	Context(ServerBlock& server, const Location& location, const fs::path path, const Uri& uri)
	: server(server), location(location), uri(uri), path(path) {}

	ServerBlock&			server;
	const Location&			location;
	Uri						uri;
	ft::filesystem::path	path;

}; /* struct Context */

Context getContext(const Uri& uri)
{
	ServerConfig& config = ServerConfig::getInstance();
	ServerBlock& server = config.findServer(uri);
	fs::path path = server.getPathFromUri(uri);
	// const Location& location = server.findLocation(path.c_str());		// working not working
	const Location& location = server.findLocation(uri);					// working not working
	return Context(server, location, path, uri);
}

#endif /* CONTEXT_HPP */