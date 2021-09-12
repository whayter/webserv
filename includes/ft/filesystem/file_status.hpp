#ifndef FILE_STATUS_HPP
#define FILE_STATUS_HPP

#include "ft/filesystem/perms.hpp"
#include "ft/filesystem/file_type.hpp"

namespace ft { 
namespace filesystem { 

class file_status {
public:
	// constructors and destructor
	file_status() throw();
	file_status(const file_status&) throw();
	file_status(file_type ft, perms prms = perms::unknown) throw();
	// file_status(file_status&&) throw();
	~file_status();
 
	// assignments
	file_status& operator=(const file_status&) throw();
	// file_status& operator=(file_status&&) throw();

	// modifiers
	void       type(file_type ft) throw();
	void       permissions(perms prms) throw();

	// observers
	file_type  type() const throw();
	perms      permissions() const throw();

	friend bool operator==(const file_status& lhs, const file_status& rhs) throw();

private:
	file_type	_type;
	perms		_perms;
}; /* class file_status */


} /* namespace filesystem */
} /* namespace ft */

#endif /* FILE_STATUS_HPP */
