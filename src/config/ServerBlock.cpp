#include "ServerBlock.hpp"

void 	ServerBlock::setAutoindex(bool autoindex) {
	_autoindex = autoindex;
	_hasAutoindex = true;
}
void 	ServerBlock::setClientMaxBodySize(size_t size) {
	_clientMaxBodySize = size;
	_hasClientMaxBodySize = true;
}
void 	ServerBlock::setReturnDirective(const ReturnDirective& returnDirective)	{
	_returnDirective = returnDirective;
	_hasReturnDirective = true;
}

Location&	ServerBlock::findLocation(const Uri& uri)
{
	(void)uri;
	return _locations[0];
}
