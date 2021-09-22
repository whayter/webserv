#ifndef DIRECTORY_ITERATOR_HPP
#define DIRECTORY_ITERATOR_HPP

#include <cstddef>
#include <iterator>
#include <sys/types.h>
#include <dirent.h>

#include "ft/filesystem/directory_entry.hpp"
#include "ft/filesystem/directory_option.hpp"
#include "ft/filesystem/path.hpp"
#include "ft/system_error.hpp"
#include "ft/error_code.hpp"
#include "ft/shared_ptr.hpp"

namespace ft {

// class error_code;

namespace filesystem {

// class path;
// class directory_entry;
// struct directory_options; // enum class

class directory_iterator {

public:

	typedef directory_entry       	value_type;
	typedef std::ptrdiff_t  		difference_type;
	typedef const directory_entry*	pointer;
	typedef const directory_entry&	reference;
	typedef std::input_iterator_tag	iterator_category;

	// member functions
	directory_iterator() throw();
	directory_iterator(const path& p);
	directory_iterator(const path& p, directory_options options);
	directory_iterator(const path& p, error_code& ec) throw();
	directory_iterator(const path& p, directory_options options, error_code& ec) throw();
	directory_iterator(const directory_iterator& other);
	// directory_iterator(directory_iterator&& rhs) throw();
	~directory_iterator();

	directory_iterator& operator=(const directory_iterator& other);
	// directory_iterator& operator=(directory_iterator&& other) throw();
	
	const directory_entry& operator*() const;
	const directory_entry* operator->() const;
	// directory_iterator&    increment(error_code& ec) throw();
	directory_iterator&    operator++();

	// other members as required by input iterators

	bool operator==(const directory_iterator& other) const;
	bool operator!=(const directory_iterator& other) const;

private:

	// path				_basePath;
	// DIR*				_dirp;
	// directory_entry		_dirEntry;
	// directory_options	_options;
	// struct dirent*		_dirent;
	// error_code			_ec;

	class impl;
	shared_ptr<impl> _impl;
}; /* class directory_iterator */

class directory_iterator::impl
{
public:
	impl(const path& p, directory_options options)
		: _basePath(p), _options(options), _dirp(NULL), _dirent(NULL) 
	{
		if (_basePath.empty())
			return ;
		_dirp = ::opendir(_basePath.c_str());
		if (_dirp == NULL)
		{
			_basePath = path();
			_ec = make_error_code();
			return ;
		}
		increment(_ec);	
	}
	~impl()
	{
		if (_dirp != NULL)
			::closedir(_dirp);
	}
    void increment(error_code& ec)
	{
		if (_dirp == NULL)
			return;
		errno = 0;
		ec.clear();
		do {
			_dirent = ::readdir(_dirp);
			if (_dirent == NULL)
			{
				::closedir(_dirp);
				if (errno)
					_ec = make_error_code();
				_dirp = NULL;
				_dirEntry._path.clear();
				return;
			}
			// _dirEntry._path = _basePath / _dirent->d_name;
			_dirEntry._path = _basePath;
			_dirEntry._path /= _dirent->d_name;
			// _dirEntry._fileSize = _dirent.
		} while (!strcmp(_dirent->d_name, "..") || !strcmp(_dirent->d_name, "."));
				
	}



	path				_basePath;
	directory_options	_options;
	DIR*				_dirp;
	struct dirent*		_dirent;
	directory_entry		_dirEntry;
	error_code			_ec;
private:
	impl(const impl& other);
}; /* class directory_iterator::impl */

}; /* namespace filesystem */

}; /* namespace ft */

#endif /* DIRECTORY_ITERATOR_HPP */
