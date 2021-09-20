/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Path.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 13:01:23 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/30 16:29:02 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_HPP
#define PATH_HPP

#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <cstring>
#include <algorithm>

namespace ft{ 
namespace filesystem{ 


class path
{
public:
	typedef std::string		string_type;


	// constructors and destructor
    path() throw();
    path(const path& other);
	path(const string_type& p);
	path(const char* p);
    // template <class InputIterator>
    //   path(InputIterator first, InputIterator last);
   ~path();


	// assignments
    path& operator=(const path& p);
    path& operator=(const string_type& p);
    // template <class InputIterator>
    //   path& assign(InputIterator first, InputIterator last);

	// appends
	path& operator/=(const path& p);
	// template <class InputIterator>
	//   path& append(InputIterator first, InputIterator last);

	// concatenation
	path& operator+=(const path& x){
		_path += x._path;
		return *this;
	}
	path& operator+=(const string_type& x){
		_path += x;
		_formatPathInPlace();
		return *this;
	}
	path& operator+=(const char* x){
		_path += x;
		_formatPathInPlace();
		return *this;
	}
	path& operator+=(char x){
		_path += x;
		_formatPathInPlace();
		return *this;
	}
	template <class Source>
      path& concat(const Source& x)
	{
		_path += x;
		return *this;
	}
	template <class InputIterator>
	path& concat(InputIterator first, InputIterator last)
	{
		while (first != last)
		{
			_path += *first;
			++first;
		}
		return *this;
	}
	// modifiers
	void  clear() throw();
	path& remove_filename();
	path& replace_filename(const path& replacement);
	path& replace_extension(const path& replacement = path());
	void  swap(path& rhs) throw();

	// native format observers
	const string_type&  native() const throw() { return _path;}
	const char*   c_str() const throw() {return _path.c_str();}
	operator string_type() const {return _path;}

	std::string    string() const{
		// std::string result;
		// const_iterator it = begin();
		// const_iterator end = this->end();

		// while(it != end)
		// 	result += *it++;
		// return result;
		return _path;
	}

    // compare
    int  compare(const path& p) const throw()
	{
		if (!this->has_root_directory() && p.has_root_directory())
			return -1;
		if (this->has_root_directory() && !p.has_root_directory())
			return +1;
		return strcmp(_path.c_str(), p._path.c_str());
	}	
    inline int  compare(const string_type& s) const
	{
		return compare(path(s));
	}
    inline int  compare(const char* s) const
	{
		return compare(path(s));
	}

    // generation
    path lexically_normal() const;
    path lexically_relative(const path& base) const;
    path lexically_proximate(const path& base) const;


	// decomposition
	path root_directory()	const;
	path root_path()		const; 
	path relative_path()	const; 
	path parent_path()		const; 
	path filename()			const; 
	path stem()				const;
	path extension()		const;

	// query
	inline bool empty() const throw()		{ return _path.empty();};
	inline bool has_root_directory() const	{ return !root_directory().empty();};
	inline bool has_root_path() const 		{ return !root_path().empty();};
	inline bool has_relative_path() const	{ return !relative_path().empty();};
	inline bool has_parent_path() const 	{ return !parent_path().empty();};
	inline bool has_filename() const 		{ return !filename().empty();};
	inline bool has_stem() const 			{ return !stem().empty();};
	inline bool has_extension() const 		{ return !extension().empty();};
	inline bool is_absolute() const 		{ return !_path.empty() && _path[0] == '/';};
	inline bool is_relative() const 		{ return !is_absolute();};


	class iterator;
	// typedef iterator const const_iterator;
	iterator 		begin() const;//			{ return iterator(*this, _path.begin());}
	iterator 		end() const;//		{ return iterator(*this, _path.end());}

private:
	friend bool operator==(const path& lhs, const path& rhs) throw();
	friend bool operator<(const path& lhs, const path& rhs) throw();

	std::vector<string_type> _splitPath();
	void _formatPathInPlace();

	string_type _path;

};

std::ostream&	operator<<(std::ostream&, const path&);

class path::iterator
{
    friend class path;
  public:
 	typedef std::ptrdiff_t					difference_type;
    typedef path							value_type;
    typedef const path&						reference;
    typedef const path*					 	pointer;
    typedef std::bidirectional_iterator_tag	iterator_category;
    typedef value_type::string_type			string_type;

    iterator();
    iterator(const path& p, const std::string::const_iterator& pos)
		: _first(p._path.begin()), _last(p._path.end()), _iter(pos)
	{
		updateCurrent();
	}
    iterator& operator++(){
		_iter = increment(_iter);
		while (_iter != _last && *_iter == '/' &&
           (_iter + 1) != _last             // the slash is not the last char
		) {
			++_iter;
		}
		updateCurrent();
		return *this;
	}
    iterator operator++(int)					{ iterator i = *this; ++(*this); return i;}
    iterator& operator--()						{ _iter = decrement(_iter); return *this;}
    iterator operator--(int)					{ iterator i = *this; --(*this); return i;}
    bool operator==(const iterator& other) const{ return _iter == other._iter;}
    bool operator!=(const iterator& other) const{ return !(*this == other);}
    reference operator*() const					{ return _cur;}
    pointer operator->() const					{ return &_cur;}

private:

    string_type::const_iterator increment(const string_type::const_iterator& pos) const;
    string_type::const_iterator decrement(const string_type::const_iterator& pos) const;
    void updateCurrent();

    string_type::const_iterator _first;
    string_type::const_iterator _last;
    // string_type::const_iterator _prefix;
    // string_type::const_iterator _root;

    string_type::const_iterator _iter;
    path _cur;
};

} /* namespace filesystem */
} /* namespace ft */

#endif /* PATH_HPP */
