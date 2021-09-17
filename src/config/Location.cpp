#include "Location.hpp"

#include <string>
#include <set>
#include "Uri.hpp"
#include "ReturnDirective.hpp"
#include "Location.hpp"

Location::Location(): _autoindex(false), _hasAutoindex(false), 
	_clientMaxBodySize(DEFAULT_CLIENT_MAX_BODY_SIZE), _hasClientMaxBodySize(false),
	_hasReturnDirective(false)
{}

void 	Location::setAutoindex(bool autoindex) {
	_autoindex = autoindex;
	_hasAutoindex = true;
}
void 	Location::setClientMaxBodySize(size_t size)	{
	_clientMaxBodySize = size;
	_hasClientMaxBodySize = true;
}
void 	Location::setReturnDirective(const ReturnDirective& returnDirective)	{
	_returnDirective = returnDirective;
	_hasReturnDirective = true;
}

void 	Location::setUri(std::string uri)				{ _uri = uri; }
void 	Location::setCgiExec(const std::string& exec)	{ _cgiExec = exec;}
void 	Location::setRoot(ft::filesystem::path root)	{ _root = root;}
void 	Location::setIndex(ft::filesystem::path index)	{ _index = index;}


void	Location::addCgiParam(const std::string& name, const std::string& value)
{
	_cgiParams[name] = value;
}
void	Location::addCgiParam(const std::pair<std::string,std::string> pair) 	
{
	_cgiParams[pair.first] = pair.second;
}

void	Location::addLimitExceptMethod(const std::string& method)
{
	_limitExceptMethods.insert(method);
}

void	Location::addLimitExceptMethods(const std::set<std::string>& l)
{
	_limitExceptMethods.insert(l.begin(), l.end());
}

bool	Location::hasLimitExceptMethods(const std::string& method)
{
	return _limitExceptMethods.count(method);
}
