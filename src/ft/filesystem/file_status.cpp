#include "ft/filesystem/file_status.hpp"

namespace ft { 
namespace filesystem { 

// constructors and destructor
file_status::file_status() throw()
	: _type(file_type::none), _perms(permsScopedEnum::unknown)
{}

file_status::file_status(const file_status& ft) throw()
	: _type(ft._type), _perms(ft._perms)
{}

file_status::file_status(file_type ft, perms prms) throw()
	: _type(ft), _perms(prms)
{}

// file_status::file_status(file_status&&) throw() {}

file_status::~file_status() {}

// assignments
file_status& file_status::operator=(const file_status& other) throw()
{
	_type = other._type;
	_perms = other._perms;
	return *this;
}

// file_status& file_status::operator=(file_status&&) throw()
// {
// 	return *this;
// }

// modifiers
void       file_status::type(file_type ft) throw()
{
	_type = ft;
}
void       file_status::permissions(perms prms) throw()
{
	_perms = prms;
}

// observers
file_type  file_status::type() const throw() {
	return _type;
}
perms      file_status::permissions() const throw() {
	return _perms;
}

bool operator==(const file_status& lhs, const file_status& rhs) throw()
{
	return lhs.type() == rhs.type() && lhs.permissions() == rhs.permissions();
}

} /* namespace filesystem */
} /* namespace ft */
