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
		std::string result;
		const_iterator it = begin();
		const_iterator end = this->end();

		while(it != end)
			result += *it++;
		return result;
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

	// iterators
	typedef string_type::iterator iterator;
	typedef string_type::const_iterator const_iterator;
	iterator 		begin() {return _path.begin();};
	iterator 		end() {return _path.end();};
	const_iterator 	begin() const	{return _path.begin();};
	const_iterator 	end() const	{return _path.end();};

private:
	friend bool operator==(const path& lhs, const path& rhs) throw();
	friend bool operator<(const path& lhs, const path& rhs) throw();

	std::vector<string_type> _splitPath();
	void _formatPathInPlace();

	string_type _path;
public:
class iterator
{
  public:
 	typedef size_t							difference_type;
    typedef path							value_type;
    typedef const path&						reference;
    typedef const path*					 	pointer;
    typedef std::bidirectional_iterator_tag	iterator_category;

    // iterator(): _M_path(nullptr), _M_cur(), _M_at_end() { }
    iterator(): _M_path(nullptr), _M_cur() { }

    iterator(const iterator&);
    iterator& operator=(const iterator&) = default;

    reference operator*() const;
    pointer   operator->() const { return std::__addressof(**this); }

    iterator& operator++();
    iterator  operator++(int) { auto __tmp = *this; ++*this; return __tmp; }

    iterator& operator--();
    iterator  operator--(int) { auto __tmp = *this; --*this; return __tmp; }

    friend bool operator==(const iterator& __lhs, const iterator& __rhs)
    { return __lhs._M_equals(__rhs); }

    friend bool operator!=(const iterator& __lhs, const iterator& __rhs)
    { return !__lhs._M_equals(__rhs); }

  private:
    friend class path;

    // bool _M_is_multi() const { return _M_path->_M_type() == _Type::_Multi; }

    // friend difference_type
    // __path_iter_distance(const iterator& __first, const iterator& __last)
    // {
    //   __glibcxx_assert(__first._M_path != nullptr);
    //   __glibcxx_assert(__first._M_path == __last._M_path);
    //   if (__first._M_is_multi())
	// return std::distance(__first._M_cur, __last._M_cur);
    //   else if (__first._M_at_end == __last._M_at_end)
	// return 0;
    //   else
	// return __first._M_at_end ? -1 : 1;
    // }

    // friend void
    // __path_iter_advance(iterator& __i, difference_type __n)
    // {
    //   if (__n == 1)
	// ++__i;
    //   else if (__n == -1)
	// --__i;
    //   else if (__n != 0)
	// {
	//   __glibcxx_assert(__i._M_path != nullptr);
	//   __glibcxx_assert(__i._M_is_multi());
	//   // __glibcxx_assert(__i._M_path->_M_cmpts.end() - __i._M_cur >= __n);
	//   __i._M_cur += __n;
	// }
    // }

    // iterator(const path* __path, path::_List::const_iterator __iter)
    // : _M_path(__path), _M_cur(__iter), _M_at_end()
    // { }

    // iterator(const path* __path, bool __at_end)
    // : _M_path(__path), _M_cur(), _M_at_end(__at_end)
    // { }

    // bool _M_equals(iterator) const;

    // const path* 		_M_path;
    // path::_List::const_iterator _M_cur;
    // bool			_M_at_end;  // only used when type != _Multi
    const path* 		_M_path;
	std::vector<path>::const_iterator _M_cur;
  };


 };



} /* namespace filesystem */
} /* namespace ft */

#endif /* PATH_HPP */
