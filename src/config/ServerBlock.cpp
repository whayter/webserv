#include "ServerBlock.hpp"
#include "filesystem.h"
#include "utility.hpp"
#include <cstdlib>
#include <string>

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

const Location*	ServerBlock::_getLocationIfMatchExtention(const Uri& uri) const
{
	std::vector<Location>::const_iterator it = _locations.begin();
	std::vector<Location>::const_iterator end = _locations.end();

	while (it != end)
	{
		const Location& loc = *it;
		if (loc.isMatchExtentionFile()
		&& loc.getExtentionFile() ==  uri.getPath().extension())
			return &loc;
		it++;
	}
	return NULL;
}

const Location&	ServerBlock::findLocation(const Uri& uri) const
{
	{
		const Location* bestMatch = _getLocationIfMatchExtention(uri);
		if (bestMatch != NULL)
			return *bestMatch;
	}
	{
		std::vector<Location>::const_iterator it = _locations.begin();
		std::vector<Location>::const_iterator end = _locations.end();
		const Location*	bestMatch = findExactLocation("/");
		size_t		bestMatchLen = bestMatch != NULL ? 1 : 0;

		while (it != end)
		{
			const Location& loc = *it;
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
				else if (len > 1 && len == bestMatchLen)
				{
					if (loc.getUri().size() > bestMatch->getUri().size())
					{
						bestMatch = &loc;
						bestMatchLen = len;
					}
				} 
			}
			it++;
		}
		if (bestMatch == NULL)
			throw std::runtime_error("Was not expecting that, need to refacto this shitty function.");
		return *bestMatch;
	}
}

const Location*	ServerBlock::findExactLocation(const Uri& uri) const
{
	std::vector<Location>::const_iterator it = _locations.begin();
	std::vector<Location>::const_iterator end = _locations.end();

	ft::filesystem::path p =  uri.getPath();
	while (it != end)
	{
		const Location& loc = *it;
		if (!p.compare(loc.getUri()))
			return &loc;
		++it;
	}
	return NULL;
}

/// if path is empty,
ft::filesystem::path	ServerBlock::getPathFromUri(const Uri& uri) const
{
	const Location&			loc = findLocation(uri);
	ft::filesystem::path	root = !loc.getRoot().empty() ? loc.getRoot() : getRoot();
	ft::filesystem::path	index = !loc.getIndex().empty() ? loc.getIndex(): getIndex();
	
	ft::filesystem::path result;
	result = root / uri.getPath().relative_path();

	if ((ft::filesystem::path(loc.getUri()) / "") == uri.getPath() / ""/"" )
	// if (loc.getUri() == uri.getPath() )
		result /= index;
	return result;
}
