#ifndef FILESYSTEM_ERROR_HPP
#define FILESYSTEM_ERROR_HPP

#include <exception>
#include <string>
#include "Path.hpp"
#include "Filesystem.hpp"

namespace ft { 

namespace filesystem {

class filesystem_error : public std::runtime_error
{
public:
	filesystem_error(const std::string& what_arg, error_code ec);
	filesystem_error(const std::string& what_arg,
		const ft::filesystem::path& p1, error_code ec);
	filesystem_error(const std::string& what_arg,
		const ft::filesystem::path& p1, const ft::filesystem::path& p2, error_code ec);

	const ft::filesystem::path& path1() const throw();
	const ft::filesystem::path& path2() const throw();
	const char* what() const throw() {
		
	}
};

} /* namespace filesystem */
} /* namespace ft */

#endif
