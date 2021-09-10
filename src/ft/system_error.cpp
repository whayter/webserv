#include "ft/filesystem/filesystem_error.hpp"
#include "ft/system_error.hpp"

namespace ft{

error_code make_error_code(int e) throw() {
	return error_code(e);
}

} /*namespace ft */
