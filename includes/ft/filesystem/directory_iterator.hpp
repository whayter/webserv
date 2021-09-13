#ifndef DIRECTORY_ITERATOR_HPP
#define DIRECTORY_ITERATOR_HPP

#include <cstddef>
#include <iterator>
#include <sys/types.h>
#include <dirent.h>

#include "ft/filesystem/directory_entry.hpp"
#include "ft/filesystem/directory_option.hpp"
#include "ft/filesystem/path.hpp"

namespace ft
{
class error_code;

namespace filesystem
{
	// class path;
	// class directory_entry;
	// struct directory_options; // enum class

	class directory_iterator
	{
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
		directory_iterator(const directory_iterator& rhs);
		// directory_iterator(directory_iterator&& rhs) throw();
		~directory_iterator();

		directory_iterator& operator=(const directory_iterator& rhs);
		directory_iterator& operator=(directory_iterator&& rhs) throw();
		
		const directory_entry& operator*() const;
		const directory_entry* operator->() const;
		directory_iterator&    operator++();
		directory_iterator&    increment(error_code& ec) throw();

		// other members as required by input iterators

		bool operator==(const directory_iterator& rhs) const;
	    bool operator!=(const directory_iterator& rhs) const;

	private:
		path				_path;
		DIR					*_dirp;
		directory_entry		_dirEntry;
		directory_options	_options;

	}; /* class directory_iterator */
}; /* namespace filesystem */
}; /* namespace ft */

#endif /* DIRECTORY_ITERATOR_HPP */
