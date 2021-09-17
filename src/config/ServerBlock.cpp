#include "ServerBlock.hpp"
#include "filesystem"

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

Location*	ServerBlock::_getLocationIfMatchExtention(const Uri& uri)
{
	std::vector<Location>::iterator it = _locations.begin();
	std::vector<Location>::iterator end = _locations.end();

	while (it != end)
	{
		Location& loc = *it;
		if (loc.isMatchExtentionFile()
		&& loc.getExtentionFile() ==  uri.getPath().extension())
			return &loc;
		it++;
	}
	return NULL;
}

Location&	ServerBlock::findLocation(const Uri& uri)
{
	std::vector<Location>::iterator it = _locations.begin();
	std::vector<Location>::iterator end = _locations.end();

	ft::filesystem::path p =  uri.getPath();

	Location* bestMatch = _getLocationIfMatchExtention(uri);
	if (bestMatch == NULL)
		while (it != end)
		{
			// const Location& loc = *it;
			
			it++;
		}
	if (bestMatch == NULL)
		return _locations[0];
	return *bestMatch;
}
