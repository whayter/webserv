#include "Filesystem.hpp"

namespace ft { namespace filesystem {

void swap(path& lhs, path& rhs)
{
	path tmp(lhs);
	lhs = rhs;
	rhs = tmp;
}

} /* namespace filesystem */
} /* namespace ft */
