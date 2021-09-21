#include "ServerBlock.hpp"
#include "filesystem.h"
#include "utility.hpp"
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
	{
		Location* bestMatch = _getLocationIfMatchExtention(uri);
		if (bestMatch != NULL)
			return *bestMatch;
	}
	{
		std::vector<Location>::iterator it = _locations.begin();
		std::vector<Location>::iterator end = _locations.end();
		Location*	bestMatch = findExactLocation("/");
		size_t		bestMatchLen = bestMatch != NULL ? 1 : 0;

		while (it != end)
		{
			Location& loc = *it;
			if (loc.isMatchExtentionFile() == false)
			{
				size_t len;
				if (ft::pathsComponentsAreEqual(loc.getUri(),  uri.getPath(), len))
					return loc;
				else if (len > bestMatchLen)
				{
					bestMatch = &loc;
					bestMatchLen = len;
				}
			}
			it++;
		}
		if (bestMatch == NULL)
			throw std::runtime_error("Was not expecting that, need to refaction this shitty function.");
		return *bestMatch;
	}
}

Location*	ServerBlock::findExactLocation(const Uri& uri)
{
	std::vector<Location>::iterator it = _locations.begin();
	std::vector<Location>::iterator end = _locations.end();

	ft::filesystem::path p =  uri.getPath();
	while (it != end)
	{
		Location& loc = *it;
		if (!p.compare(loc.getUri()))
			return &loc;
		++it;
	}
	return NULL;
}
