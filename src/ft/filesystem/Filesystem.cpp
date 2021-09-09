#include "ft/filesystem/filesystem.hpp"

#include "ft/filesystem/filesystem_error.hpp"

namespace ft { namespace filesystem {

void swap(path& lhs, path& rhs) throw() 
{
	path tmp(lhs);
	lhs = rhs;
	rhs = tmp;
}

path current_path()
{
	std::string result;
	char cwd[PATH_MAX];
	// if (!getcwd(cwd, sizeof(cwd)))
	// 	throw filesystem_error("getcwd() failed");
	result = cwd;
	return path(result);
}
path current_path(error_code& ec)
{
	(void)ec;
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
