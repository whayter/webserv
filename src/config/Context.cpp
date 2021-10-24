#include "Context.hpp"

Context getContext(const Uri& uri)
{
	ServerConfig& config = ServerConfig::getInstance();
	ServerBlock& server = config.findServer(uri);
	std::pair<const Location *,  fs::path> res = server.getPathFromUri(uri);
	return Context(server, *res.first, res.second, uri);
}