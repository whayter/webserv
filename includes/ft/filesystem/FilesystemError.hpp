#ifndef FILESYSTEM_ERROR_HPP
#define FILESYSTEM_ERROR_HPP

#include <exception>
#include <string>
#include "ft/error_code.hpp"
#include "ft/filesystem/Path.hpp"

namespace ft { 

namespace filesystem {

	class filesystem_error : public std::runtime_error
	{
	public:
		filesystem_error(const std::string& what_arg, ft::error_code ec)
			:  std::runtime_error(what_arg), _code(ec) {}
		filesystem_error(const std::string& what_arg, const path& p1, error_code ec)
			:  std::runtime_error(what_arg), _path1(p1), _code(ec) {}
		filesystem_error(const std::string& what_arg, const path& p1, const path& p2, error_code ec)
			:  std::runtime_error(what_arg), _path1(p1), _path2(p2), _code(ec) {}
		virtual ~filesystem_error() throw() {}

		const path&			path1() const throw()	{ return _path1;}
		const path& 		path2() const throw()	{ return _path2;}
		const error_code&	code() const throw()	{ return _code;}

	private:
		path 		_path1;
		path 		_path2;
		error_code	_code;
	};

} /* namespace filesystem */
} /* namespace ft */

#endif /* FILESYSTEM_ERROR_HPP */
