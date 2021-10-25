#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <set>
#include "Uri.hpp"
#include "ReturnDirective.hpp"

#define DEFAULT_CLIENT_MAX_BODY_SIZE 1000 * 1000 // = 1m


struct action{
    enum actionEnum {
      none = 0,
	  returnDirective,
	  cgi
    };

    action(actionEnum e): _e(e) {}
    // operator file_typeEnum() const throw(){
    //   return _e;
    // }
    friend bool operator==(const action& lhs, const action& rhs){
      return lhs._e == rhs._e;
    }
    friend bool operator!=(const action& lhs, const action& rhs){
      return lhs._e != rhs._e;
    }
private:
  actionEnum _e;
};

class Location
{
public:
	Location();

	inline std::string				getUri() const;
	inline std::string				getExtentionFile() const;
	inline bool						isMatchExtentionFile() const;

	inline bool						getAutoindex() const;
	inline action					getAction() const;

	inline std::string				getCgiExec() const;
	inline size_t					getClientMaxBodySize() const;
	inline const ReturnDirective&	getReturnDirective() const;
	inline ft::filesystem::path		getRoot() const;
	inline ft::filesystem::path		getIndex() const;

	inline bool						hasAutoindex() const;
	inline bool						hasClientMaxBodySize() const;
	inline bool						hasReturnDirective() const;

	void 	setAutoindex(bool autoindex);
	void 	setClientMaxBodySize(size_t size);
	void 	setReturnDirective(const ReturnDirective& returnDirective);

	void 	setUri(std::string uri);
	void 	setExtentionFile(std::string ext);
	void 	setCgiExec(const std::string& exec);
	void 	setRoot(ft::filesystem::path root);
	void 	setIndex(ft::filesystem::path index);

	void	addCgiParam(const std::string& name, const std::string& value);
	void	addCgiParam(const std::pair<std::string,std::string> pair);

	/// return the map of cgiParam (usefull for testing purpose)
	inline std::map<std::string, std::string>&	getCgiParams();
	inline std::string							getCgiParam(std::string param);

	void	addLimitExceptMethod(const std::string& method);
	void	addLimitExceptMethods(const std::set<std::string>& l);
	inline	std::set<std::string>&	getLimitExceptMethods();
	bool	hasLimitExceptMethod(const std::string& method) const;

private:
	void _setAction(action);


	action _action;
	
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

inline std::string				Location::getUri() const				{ return _uri; }
inline std::string				Location::getExtentionFile() const		{ return _extentionFile; }
inline bool						Location::isMatchExtentionFile() const	{ return _uri.empty(); }

inline bool						Location::getAutoindex() const		{ return _autoindex; }
inline action					Location::getAction() const 		{ return _action;}

inline std::string				Location::getCgiExec() const			{ return _cgiExec; }
inline size_t					Location::getClientMaxBodySize() const	{ return _clientMaxBodySize; }
inline const ReturnDirective&	Location::getReturnDirective() const	{ return _returnDirective; }
inline ft::filesystem::path		Location::getRoot() const				{ return _root; }
inline ft::filesystem::path		Location::getIndex() const				{ return _index; }

inline bool						Location::hasAutoindex() const			{ return _hasAutoindex; }
inline bool						Location::hasClientMaxBodySize() const	{ return _hasClientMaxBodySize; }
inline bool						Location::hasReturnDirective() const	{ return _hasReturnDirective; }

inline std::map<std::string, std::string>&	Location::getCgiParams()
{
	return _cgiParams;
}
inline std::string		Location::getCgiParam(std::string param)
{
	return _cgiParams[param];
}

inline	std::set<std::string>&	Location::getLimitExceptMethods()
{
	return _limitExceptMethods;
}

#endif /* LOCATION_HPP */
