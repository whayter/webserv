#include "Filesystem.hpp"

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
	if (!getcwd(cwd, sizeof(cwd)))
		throw std::runtime_error("getcwd() failed");
	result = cwd;
	return path(result);
}
path current_path(error_code& ec)
{
	(void)ec;
	return path();
}


} /* namespace filesystem */
} /* namespace ft */
