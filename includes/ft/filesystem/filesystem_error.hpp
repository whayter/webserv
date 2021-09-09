#ifndef FILESYSTEM_ERROR_HPP
#define FILESYSTEM_ERROR_HPP

#include <exception>
#include <string>
#include "ft/error_code.hpp"
#include "ft/filesystem/path.hpp"

namespace ft { 

namespace filesystem {

class filesystem_error : public std::runtime_error
{
public:
	filesystem_error(const std::string& what_arg, ft::error_code ec)
		: std::runtime_error(what_arg), _code(ec) {}
	filesystem_error(const std::string& what_arg, const path& p1, ft::error_code ec)
		: std::runtime_error(what_arg), _path1(p1), _code(ec) {}
	filesystem_error(const std::string& what_arg, const path& p1, const path& p2, ft::error_code ec)
		: std::runtime_error(what_arg), _path1(p1), _path2(p2), _code(ec) {}
	filesystem_error( const filesystem_error& other ) throw()
		: std::runtime_error(other.what()), _path1(other._path1), _path2(other._path2), _code(other._code) {}

	virtual ~filesystem_error() throw() {}

	const path&				path1() const throw()	{ return _path1;}
	const path& 			path2() const throw()	{ return _path2;}
	const ft::error_code&	code() const throw()	{ return _code;}

	const char* what() const throw() { return std::runtime_error::what();}

private:
	path 			_path1;
	path 			_path2;
	ft::error_code	_code;
};

} /* namespace filesystem */
} /* namespace ft */

#endif /* FILESYSTEM_ERROR_HPP */
