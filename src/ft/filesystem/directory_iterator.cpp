#include "ft/filesystem/directory_iterator.hpp"

#include "ft/filesystem/filesystem.hpp"
#include "ft/filesystem/path.hpp"
#include "ft/filesystem/filesystem_error.hpp"
#include "ft/filesystem/directory_option.hpp"
#include "ft/system_error.hpp"

namespace ft {

namespace filesystem {

directory_iterator::directory_iterator() throw()
	: _impl(new impl(path(), directory_options::none))
{}

directory_iterator::directory_iterator(const path &p)
	: _impl(new impl(p, directory_options::none))
{
	error_code& ec = _impl->_ec;
	if (ec)
		throw filesystem_error("directory_iterator(const path &p) : " + ec.message(), _impl->_basePath, ec);
}

directory_iterator::directory_iterator(const path &p, directory_options options)
	: _impl(new impl(p, options))
{
	error_code& ec = _impl->_ec;
	if (ec)
		throw filesystem_error("directory_iterator(const path &p, directory_options options) : " + ec.message(), _impl->_basePath, ec);
}

directory_iterator::directory_iterator(const path &p, error_code &ec) throw()
	: _impl(new impl(p, directory_options::none))
{
	ec = _impl->_ec;
}

directory_iterator::directory_iterator(const path &p, directory_options options, error_code &ec) throw()
	: _impl(new impl(p, options))
{
	ec = _impl->_ec;
}

directory_iterator::directory_iterator(const directory_iterator &other)
	: _impl(other._impl)
{}

directory_iterator::~directory_iterator() {}

directory_iterator& directory_iterator::operator=(const directory_iterator &other)
{
	_impl =  other._impl;
	return *this;
}

const directory_entry& directory_iterator::operator*() const	{ return _impl->_dirEntry;}
const directory_entry* directory_iterator::operator->() const	{ return &_impl->_dirEntry;}

directory_iterator& directory_iterator::operator++()
{
	error_code ec;
	_impl->increment(ec);
	if (ec)
		throw filesystem_error("directory_iterator::operator++(): " + ec.message(), _impl->_basePath / _impl->_dirEntry.path() , ec);
	return *this;
}

bool directory_iterator::operator==(const directory_iterator& other) const
{
	return _impl->_dirEntry._path == other._impl->_dirEntry._path;
}

bool directory_iterator::operator!=(const directory_iterator& other) const
{
	return !this->operator==(other);
}

}; /* namespace filesystem */
}; /* namespace ft */
