#include "ft/filesystem/directory_iterator.hpp"

#include "ft/filesystem/filesystem.hpp"
#include "ft/filesystem/path.hpp"
#include "ft/filesystem/filesystem_error.hpp"
#include "ft/filesystem/directory_option.hpp"
#include "ft/system_error.hpp"



namespace ft
{
namespace filesystem
{

directory_iterator::directory_iterator() throw()
	: _basePath(path()), _dirp(0), _dirEntry(directory_entry()), _options(directory_options::none)
{}

directory_iterator::directory_iterator(const path &p)
	: _basePath(p), _dirp(0), _dirEntry(directory_entry()), _options(directory_options::none)
{
	if (_basePath.empty())
		return ;
	_dirp = ::opendir(_basePath.c_str());
	if (_dirp == NULL)
	{
		_ec = make_error_code();
		if (_ec)
			throw filesystem_error("directory_iterator::directory_iterator(const path& p=\""+ p.string() +"\"): " + _ec.message(), p, _ec);
	}
			
}

directory_iterator::directory_iterator(const path &p, directory_options options)
	: _basePath(p), _dirp(0), _dirEntry(directory_entry()), _options(options)
{

}

directory_iterator::directory_iterator(const path &p, error_code &ec) throw()
	: _basePath(p), _dirp(0), _dirEntry(directory_entry()), _options(directory_options::none)
{
	(void)ec;
}
directory_iterator::directory_iterator(const path &p, directory_options options, error_code &ec) throw()
	: _basePath(p), _dirp(0), _dirEntry(directory_entry()), _options(options)
{
	(void)ec;
}
directory_iterator::directory_iterator(const directory_iterator &other)
	: _basePath( other._basePath), _dirp(other._dirp), 
	_dirEntry(other._dirEntry), _options(other._options), _dirent(other._dirent)
{}

// directory_iterator::directory_iterator(directory_iterator &&rhs) throw()
// {

// }

directory_iterator::~directory_iterator()
{
	if(_dirp == NULL)
		return;
	::closedir(_dirp);
	_dirp = NULL;
}

directory_iterator& directory_iterator::operator=(const directory_iterator &other)
{
	_basePath = other._basePath;
	_dirp = other._dirp;
	_dirEntry = other._dirEntry;
	_options = other._options;
	_dirent = other._dirent;
	return *this;
}
// directory_iterator& directory_iterator::operator=(directory_iterator &&rhs) throw() {}

const directory_entry& directory_iterator::operator*() const
{
	return _dirEntry;
}
const directory_entry* directory_iterator::operator->() const
{
	return &_dirEntry;
}
directory_iterator& directory_iterator::operator++()
{
	// _dirent = ::readdir(_dirp);
	// if (_dirent == NULL)
	// {
	// 	this->~directory_iterator();
	// 	return ;
	// }
	error_code ec;
	directory_iterator& result = increment(ec);
	if (ec)
		throw filesystem_error("directory_iterator::operator++(): " + ec.message(), _basePath / _dirEntry.path() , ec);
	return result;
}
directory_iterator& directory_iterator::increment(error_code &ec) throw()
{
	errno = 0;
	_dirent = ::readdir(_dirp);
	if (_dirent == NULL)
	{
		if (errno)
			ec = make_error_code();
		this->~directory_iterator();
		return *this;
	}
	ec.clear();
	_dirEntry._path = _basePath / _dirent->d_name;
	// _dirEntry._fileSize = _dirent.

	return *this;
}


bool directory_iterator::operator==(const directory_iterator& other) const
{
	return _dirEntry._path == other._dirEntry._path;
}
bool directory_iterator::operator!=(const directory_iterator& other) const
{
	return !this->operator==(other);
}



}; /* namespace filesystem */
}; /* namespace ft */
