#ifndef SERVER_BLOCK_HPP
#define SERVER_BLOCK_HPP

#include "filesystem.h"
#include "Location.hpp"

#include "Host.hpp"

class ServerBlock
{
public:
	ServerBlock(): _autoindex(false), _hasAutoindex(false),
		_clientMaxBodySize(DEFAULT_CLIENT_MAX_BODY_SIZE), _hasClientMaxBodySize(false),
		_hasReturnDirective(false) {}

	inline ft::filesystem::path		getIndex() const			{ return _index; }
	inline bool						getAutoindex() const		{ return _autoindex; }
	inline ft::filesystem::path		getRoot() const				{ return _root; }
	inline std::string				getServerName() const		{ return _serverName; }
	inline size_t					getClientMaxBodySize() const{ return _clientMaxBodySize; }
	inline const ReturnDirective&	getReturnDirective() const	{ return _returnDirective; }

	inline bool		hasAutoindex() const		{ return _hasAutoindex; }
	inline bool		hasClientMaxBodySize() const{ return _hasClientMaxBodySize; }
	inline bool		hasReturnDirective() const	{ return _hasReturnDirective; }

	void	setAutoindex(bool autoindex);
	void	setClientMaxBodySize(size_t size);
	void	setReturnDirective(const ReturnDirective& returnDirective);

	/// return listen from given index (usefull for testing purpose)
	inline const Host&				getListen(uint32_t index) const	{ return _listens[index];}
	/// return the vector of listens (usefull for testing purpose)
	inline std::vector<Host>&		getListens() 			{ return _listens;}
	/// return the vector of locations (usefull for testing purpose)
	inline std::vector<Location>&	getLocations() 			{ return _locations;}
	/// return the map of errors (usefull for testing purpose)
	inline std::map<u_short, std::string>&	getErrors() 	{ return _errors;}

	void	addListen(Host listen) {_listens.push_back(listen);}
	void	addLocation(Location location) {_locations.push_back(location);}
	void	addError(u_short code, const std::string& path) {_errors[code] = path;}
	void	addErrors(const std::map<u_short, std::string>& errors) { _errors.insert(errors.begin(), errors.end());}
	

	void 	setIndex(ft::filesystem::path index)				{ _index = index;}
	void 	setRoot(ft::filesystem::path root)					{ _root = root;}
	void 	setServerName(std::string serverName)				{ _serverName = serverName;}

	const Location&				findLocation(const Uri& uri) const;
	const Location*				findExactLocation(const Uri& uri) const;
	ft::filesystem::path	getPathFromUri(const Uri& uri) const;

private:
	const Location*	_getLocationIfMatchExtention(const Uri& uri) const;

	std::vector<Host>					_listens;
	std::vector<Location>				_locations;
	std::map<u_short, std::string> 		_errors;
	ft::filesystem::path				_index;

	bool								_autoindex;
	bool								_hasAutoindex;
	
	size_t								_clientMaxBodySize;
	bool								_hasClientMaxBodySize;
	
	ReturnDirective						_returnDirective;
	bool								_hasReturnDirective;

	std::string							_serverName;
	ft::filesystem::path				_root;
}; /* class ServerBlock */

#endif /* SERVER_BLOCK_HPP */
