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
	filesystem_error(const std::string& what_arg, ft::error_code ec);
	filesystem_error(const std::string& what_arg, const path& p1, ft::error_code ec);
	filesystem_error(const std::string& what_arg, const path& p1, const path& p2, ft::error_code ec);
	filesystem_error( const filesystem_error& other ) throw();

	virtual ~filesystem_error() throw();

	const path&				path1() const throw();
	const path& 			path2() const throw();
	const ft::error_code&	code() const throw();

	const char* what() const throw();

private:
	path 			_path1;
	path 			_path2;
	ft::error_code	_code;
};

} /* namespace filesystem */
} /* namespace ft */

#endif /* FILESYSTEM_ERROR_HPP */
