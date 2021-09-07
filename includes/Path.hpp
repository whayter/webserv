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

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <uuid/uuid.h>


class Path
{
public:
	typedef std::string		string_type;


	// constructors and destructor
    Path();
    Path(const Path& other);
	Path(const string_type& p);
    // template <class InputIterator>
    //   Path(InputIterator first, InputIterator last);
   ~Path();


	// assignments
    Path& operator=(const Path& p);
    // template <class InputIterator>
    //   Path& assign(InputIterator first, InputIterator last);

	// appends
	Path& operator/=(const Path& p);
	// template <class InputIterator>
	//   Path& append(InputIterator first, InputIterator last);

	// concatenation
	Path& operator+=(const Path& x){
		_path += x._path;
		return *this;
	}
	Path& operator+=(const string_type& x){
		_path += x;
		return *this;
	}
	Path& operator+=(const char* x){
		_path += x;
		return *this;
	}
	Path& operator+=(char x){
		_path += x;
		return *this;
	}
	template <class InputIterator>
	  Path& concat(InputIterator first, InputIterator last)
	{
		while (first != last)
		{
			_path += *first;
			++first;
		}
	}
	// modifiers
	void  clear() { _path.empty();}
	// Path& remove_filename();
	// Path& replace_filename(const Path& replacement);
	// Path& replace_extension(const Path& replacement = Path());
	// void  swap(Path& rhs) noexcept;

	// native format observers
	const string_type&  native() const { return _path;}
	const char*   c_str() const {return _path.c_str();}
	operator string_type() const {return _path;}

	std::string    string() const{
		std::string result;
		const_iterator it = begin();
		const_iterator end = this->end();

		while(it != end)
			result += *it++;
		return result;
	}

	// decomposition
	Path root_directory()	const;
	Path root_path()		const; 
	Path relative_path()	const; 
	Path parent_path()		const; 
	Path filename()			const; 
	Path stem()				const;
	Path extension()		const;

	// query
	inline bool empty() const		 		{ return _path.empty();};
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
	// class iterator;
	// typedef iterator const_iterator;
	typedef string_type::iterator iterator;
	typedef string_type::const_iterator const_iterator;

	iterator 		begin() {return _path.begin();}; // why it should be const ??
	iterator 		end() {return _path.end();}; // why it should be const ??
	const_iterator 	begin() const	{return _path.begin();}; // why it should be const ??
	const_iterator 	end() const	{return _path.end();}; // why it should be const ??

private:


	friend bool operator==(const Path& lhs, const Path& rhs);
	friend bool operator<(const Path& lhs, const Path& rhs);

	void _parsePath(const string_type&);

	string_type _path;
};


inline bool operator==(const Path& lhs, const Path& rhs){
	return lhs._path == rhs._path;
}
inline bool operator< (const Path& lhs, const Path& rhs){
	return lhs._path < rhs._path;
}

// move this to filesystem file
inline bool operator!=(const Path& lhs, const Path& rhs) { return !(lhs == rhs); }
inline bool operator<=(const Path& lhs, const Path& rhs) { return !(rhs < lhs); }
inline bool operator> (const Path& lhs, const Path& rhs) { return rhs < lhs; }
inline bool operator>=(const Path& lhs, const Path& rhs) { return !(lhs < rhs); }
// Path		operator/ (const Path& lhs, const Path& rhs)		{	return Path(lhs) /= rhs;}












// #include <limits.h> for linux

#define PATH_MAX 1024 // For linux, don't care for now. Override macos define (but same len)

// https://man7.org/linux/man-pages/man7/path_resolution.7.html
// https://insanecoding.blogspot.com/2007/11/pathmax-simply-isnt.html
// https://www.gnu.org/software/libc/manual/html_node/Limits-for-Files.html

// class Path
// {
// public:

// 	/// @brief Create empty Path
// 	Path();
// 	/// @brief Creates an empty absolute or relative path.
// 	Path(bool absolute);
// 	/// @brief Create Path from string
// 	Path(const std::string& path);
// 	/// @brief Copy constructor
// 	Path(const Path&);
// 	/// @brief Assignment operator
// 	Path& operator=(const Path&);
// 	/// @brief Parse string and assign it
// 	Path& operator=(const std::string& path);

// 	~Path();


// 	// /// Returns the filename.
// 	// std::string		getFilename() const;
// 	// /// Returns the basename (filename without extension)
// 	// std::string		getBaseName() const;
// 	// /// Returns the filename extension.
// 	// std::string		getExtention() const;


// 	/// Returns true if the path refer to a directory (the filename part is empty).
// 	inline bool		isDirectory() const { return _filename.empty();}
// 	/// Returns true if the path refer to a file (the filename part is not empty).
// 	inline bool		isFile() const {return !_filename.empty();}
// 	/// Returns true if the path is absolute.
// 	inline bool		isAbsolute() const { return _absolute;}
// 	/// Returns true if the path is relative.
// 	inline bool		isRelative() const { return !_absolute;}


// 	inline size_t	depth() const { return _dirs.size();}


// 	/// Makes the path absolute if it is relative. The current working directory is taken as base directory.
// 	Path & makeAbsolute();


// 	/// Returns the user's home directory.
// 	static std::string home();
// 	/// Returns the current working directory.
// 	static std::string current();
// 	/// Returns the temporary directory.
// 	static std::string temp();


// 	/// Returns an absolute variant of the path, taking the current working directory as base.
// 	Path absolute() const;
// 	/// Returns an absolute variant of the path, taking the given path as base.
// 	Path absolute(const Path & base) const;


// 	void	clear();
// 	std::string toString() const;

// 	void parse(const std::string& path);

// private:

// 	std::string					_filename;
// 	std::vector<std::string>	_dirs; // list of previous directories
// 	bool						_absolute;
// };

#endif /* PATH_HPP */
