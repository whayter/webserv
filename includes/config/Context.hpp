#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "ServerConfig.hpp"

struct Context {

	ServerBlock				server;
	Location				location;
	Uri						uri;
	ft::filesystem::path	path;

}; /* struct Context */

Context getContext(const Uri& uri)
{
	Context ctxt;
	ServerConfig& config = ServerConfig::getInstance();
	ctxt.uri = uri;
	ctxt.server = config.findServer(ctxt.uri);
	ctxt.path = ctxt.server.getPathFromUri(ctxt.uri);
	ctxt.location = ctxt.server.findLocation(ctxt.path.c_str());
	return ctxt;
}

#endif /* CONTEXT_HPP */