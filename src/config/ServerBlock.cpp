#include "ServerBlock.hpp"
#include "filesystem.hpp"
#include <cstdlib>

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

	{
		Location* bestMatch = _getLocationIfMatchExtention(uri);
		if (bestMatch != NULL)
			return *bestMatch;
	}
	{
		Location&	bestMatch = _locations[0];
		int			bestMatchLen = 0;

		ft::filesystem::path p =  uri.getPath();
		while (it != end)
		{
			Location& loc = *it;
			if (loc.isMatchExtentionFile() == false)
			{
				int i = ::abs(loc.getUri().compare(p.string()));
				std::cout << loc.getUri() << " vs " << p.string() << std::endl;
				if (i > bestMatchLen)
				{
					bestMatch = loc;
					bestMatchLen = i;
				}
			}
			it++;
		}
		return bestMatch;
	}
}
