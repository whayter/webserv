#include "ft/filesystem/filesystem.hpp"

#include "ft/filesystem/filesystem_error.hpp"
#include "ft/system_error.hpp"
#include "ft/error_code.hpp"

#include <unistd.h>

namespace ft { namespace filesystem {

void swap(path& lhs, path& rhs) throw() 
{
	path tmp(lhs);
	lhs = rhs;
	rhs = tmp;
}

bool operator==(const path& lhs, const path& rhs) throw() {return lhs._path == rhs._path;}
bool operator< (const path& lhs, const path& rhs) throw() {return lhs._path < rhs._path;}
bool operator!=(const path& lhs, const path& rhs) throw() { return !(lhs == rhs); }
bool operator<=(const path& lhs, const path& rhs) throw() { return !(rhs < lhs); }
bool operator> (const path& lhs, const path& rhs) throw() { return rhs < lhs; }
bool operator>=(const path& lhs, const path& rhs) throw() { return !(lhs < rhs); }
path operator/ (const path& lhs, const path& rhs) { return path(lhs) /= rhs;}
 
path current_path()
{
	std::string result;
	char cwd[PATH_MAX];
	if (!getcwd(cwd, sizeof(cwd)))
		throw filesystem_error("getcwd() failed", make_error_code(errc::none));
	result = cwd;
	return path(result);
}
path current_path(error_code& ec)
{
	(void)ec;
	std::string result;
	char cwd[PATH_MAX];
	if (!getcwd(cwd, sizeof(cwd)))
		ec = make_error_code(errc::none);
	else
		ec.clear();
	result = cwd;
	return path(result);
	return path();
}

void current_path(const path& p)
{
	(void)p;
}
void current_path(const path& p, error_code& ec) throw()
{
	(void)p;
	(void)ec;
}


} /* namespace filesystem */
} /* namespace ft */
