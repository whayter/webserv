#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <set>
#include "Uri.hpp"
#include "ReturnDirective.hpp"

#define DEFAULT_CLIENT_MAX_BODY_SIZE 1000 * 1000 // = 1m

class Location
{
public:
	Location(): _autoindex(false), _hasAutoindex(false), 
		_clientMaxBodySize(DEFAULT_CLIENT_MAX_BODY_SIZE), _hasClientMaxBodySize(false),
		_hasReturnDirective(false)
	{}
	// Location (const Location& other):
	// 	_uri(other._uri),
	// 	_autoindex(other._autoindex),
	// 	_hasAutoindex(other._hasAutoindex),
	// 	_clientMaxBodySize(other._clientMaxBodySize),
	// 	_hasClientMaxBodySize(other._hasClientMaxBodySize),
	// 	_returnDirective(other._returnDirective),
	// 	_hasReturnDirective(other._hasReturnDirective),
	// 	_cgiExec(other._cgiExec),
	// 	_cgiParams(other._cgiParams),
	// 	_limitExceptMethods(other._limitExceptMethods),
	// 	_root(other._root),
	// 	_index(other._index)
	// {}

	inline std::string				getUri() const				{ return _uri; }
	inline std::string				getExtentionFile() const	{ return _extentionFile; }
	inline bool						isMatchExtentionFile() const{ return _uri.empty(); }

	inline bool						getAutoindex() const		{ return _autoindex; }

	inline std::string				getCgiExec() const		{ return _cgiExec; }
	inline size_t					getClientMaxBodySize() const{ return _clientMaxBodySize; }
	inline const ReturnDirective&	getReturnDirective() const	{ return _returnDirective; }
	inline ft::filesystem::path		getRoot() const				{ return _root; }
	inline ft::filesystem::path		getIndex() const			{ return _index; }

	inline bool						hasAutoindex() const		{ return _hasAutoindex; }
	inline bool						hasClientMaxBodySize() const{ return _hasClientMaxBodySize; }
	inline bool						hasReturnDirective() const	{ return _hasReturnDirective; }

	void 	setAutoindex(bool autoindex) {
		 _autoindex = autoindex;
		 _hasAutoindex = true;
	}
	void 	setClientMaxBodySize(size_t size)	{
		_clientMaxBodySize = size;
		_hasClientMaxBodySize = true;
	}
	void 	setReturnDirective(const ReturnDirective& returnDirective)	{
		_returnDirective = returnDirective;
		_hasReturnDirective = true;
	}
	void 	setUri(std::string uri)					{ _uri = uri; }
	void 	setCgiExec(const std::string& exec)		{ _cgiExec = exec;}
	void 	setRoot(ft::filesystem::path root)		{_root = root;}
	void 	setIndex(ft::filesystem::path index)	{_index = index;}

	void	addCgiParam(const std::string& name, const std::string& value) { _cgiParams[name] = value;}
	void	addCgiParam(const std::pair<std::string,std::string> pair) 	{ _cgiParams[pair.first] = pair.second;}

	/// return the map of cgiParam (usefull for testing purpose)
	inline std::map<std::string, std::string>&	getCgiParams() 	{ return _cgiParams;}
	inline std::string	getCgiParam(std::string param) 			{ return _cgiParams[param];}

	void	addLimitExceptMethod(const std::string& method)			{_limitExceptMethods.insert(method);}
	void	addLimitExceptMethods(const std::set<std::string>& l)	{ _limitExceptMethods.insert(l.begin(), l.end());}
	inline	std::set<std::string>&	getLimitExceptMethods() 		{ return _limitExceptMethods;}
	bool	hasLimitExceptMethods(const std::string& method)		{ return _limitExceptMethods.count(method);}

private:
	std::string							_uri;
	std::string							_extentionFile;
	
	bool								_autoindex;
	bool								_hasAutoindex;
	
	size_t								_clientMaxBodySize;
	bool								_hasClientMaxBodySize;

	ReturnDirective						_returnDirective;
	bool								_hasReturnDirective;

	std::string							_cgiExec;
	std::map<std::string, std::string> 	_cgiParams;

	std::set<std::string>				_limitExceptMethods;

	ft::filesystem::path				_root;
	ft::filesystem::path				_index;
}; /* class Location */

#endif /* LOCATION_HPP */
