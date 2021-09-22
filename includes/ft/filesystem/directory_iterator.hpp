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

namespace filesystem {

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
	~directory_iterator();

	directory_iterator&		operator=(const directory_iterator& other);	
	const directory_entry&	operator*() const;
	const directory_entry*	operator->() const;
	directory_iterator&		operator++();

	// other members as required by input iterators
	bool operator==(const directory_iterator& other) const;
	bool operator!=(const directory_iterator& other) const;

private:
	class impl;
	shared_ptr<impl> _impl;
}; /* class directory_iterator */

class directory_iterator::impl
{
public:
	impl(const path& p, directory_options options);
	~impl();

	void increment(error_code& ec);

	path				_basePath;
	directory_options	_options;
	DIR*				_dirp;
	struct dirent*		_dirent;
	directory_entry		_dirEntry;
	error_code			_ec;
private:
	impl(const impl& other);
	void _copyEntryFromDirent();
}; /* class directory_iterator::impl */

}; /* namespace filesystem */

}; /* namespace ft */

#endif /* DIRECTORY_ITERATOR_HPP */
