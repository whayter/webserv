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


bool operator==(const path& lhs, const path& rhs) noexcept{}
bool operator!=(const path& lhs, const path& rhs) noexcept;
bool operator< (const path& lhs, const path& rhs) noexcept;
bool operator<=(const path& lhs, const path& rhs) noexcept;
bool operator> (const path& lhs, const path& rhs) noexcept;
bool operator>=(const path& lhs, const path& rhs) noexcept;

class Path
{
public:
	typedef std::basic_string<char> 	string_type;
	static constexpr char   		preferred_separator = '/';

	typedef std::vector<string_type> string_vec;
	typedef string_vec::iterator iterator;
	typedef string_vec::const_iterator const_iterator;


	// constructors and destructor
	Path();
	Path(const Path& p);
	Path(const string_type& p);
	// Path(Path&& p) noexcept;
	// template <class Source>
	//   Path(const Source& source);
	// template <class InputIterator>
	//   Path(InputIterator first, InputIterator last);
	// template <class Source>  
	//   Path(const Source& source, const locale& loc);
	// template <class InputIterator>
	//   Path(InputIterator first, InputIterator last, const locale& loc);
   ~Path();

	// assignments
	Path& operator=(const Path& other);
	// Path& operator=(Path&& p) noexcept;
	Path& operator=(const string_type& path);
	Path& assign(const string_type& path);
	// template <class InputIterator>
	//   Path& assign(InputIterator first, InputIterator last);

	// appends
	// Path& operator/=(const Path& p);
	// template <class Source>
	//   Path& operator/=(const Source& source);
	// template <class Source>
	//   Path& append(const Source& source);
	// template <class InputIterator>
	//   Path& append(InputIterator first, InputIterator last);

	// concatenation
	// Path& operator+=(const Path& x);
	// Path& operator+=(const string_type& x);
	// Path& operator+=(const char* x);
	// Path& operator+=(char x);
	// template <class Source>
	//   Path& operator+=(const Source& x);
	// template <class EcharT>
	//   Path& operator+=(EcharT x);
	// template <class Source>
	//   Path& concat(const Source& x);
	// template <class InputIterator>
	//   Path& concat(InputIterator first, InputIterator last);
	
	// modifiers
	// void  clear() noexcept;
	// Path& make_preferred();
	// Path& remove_filename();
	// Path& replace_filename(const Path& replacement);
	// Path& replace_extension(const Path& replacement = Path());
	// void  swap(Path& rhs) noexcept;

	// native format observers
	// const string_type&  native() const noexcept;
	// const char*   c_str() const noexcept;
	// operator string_type() const;

	// template <class EcharT, class traits = std::char_traits<EcharT>,
	// 		  class Allocator = std::allocator<EcharT> >
	// std::basic_string<EcharT, traits, Allocator>
	//   string(const Allocator& a = Allocator()) const;
	// std::string    string() const;

	// compare
	int  compare(const Path& p) const noexcept
	{
		
	}
	// int  compare(const string_type& s) const;
	// int  compare(const char* s) const;

	// decomposition
	// Path root_name() 		const { return *this;};  useless for unix
	Path root_directory()	const { return !_pathEntries.empty() && _pathEntries[0] == "/" ? Path(_pathEntries[0]) : Path() ; }
	Path root_path()		const { return /* root_name() / */ root_directory(); }
	Path relative_path()	const { return empty() ? Path() : Path(*--end());}
	Path parent_path()		const { return (empty() || begin() == --end()) ? Path() : Path(*this); }
	Path filename()			const { return empty() ? Path() : Path(*--end()); }
	Path stem()				const
	{
		if (empty())
			return Path();
		std::string filename = *--end();
		if (filename == "." || filename == "..")
			return Path();
		return Path(filename.substr(0, filename.rfind('.')));
	}
	Path extension()		const 
	{
		if (empty())
			return Path();
		std::string filename = *--end();
		if (filename == "." || filename == "..")
			return Path();
		return Path(filename.substr(filename.rfind('.')));
	}

	// query
	inline bool empty() const noexcept 		{ return _pathEntries.empty();};
	// inline bool has_root_name() const		{ return !root_name().empty();};
	inline bool has_root_directory() const	{ return !root_directory().empty();};
	inline bool has_root_path() const 		{ return !root_path().empty();};
	inline bool has_relative_path() const	{ return !relative_path().empty();};
	inline bool has_parent_path() const 	{ return !parent_path().empty();};
	inline bool has_filename() const 		{ return !filename().empty();};
	inline bool has_stem() const 			{ return !stem().empty();};
	inline bool has_extension() const 		{ return !extension().empty();};
	inline bool is_absolute() const 		{ return !_pathEntries.empty() && _pathEntries[0] == "/";};
	inline bool is_relative() const 		{ return !is_absolute();};

	// iterators
	// class iterator;
	// typedef iterator const_iterator;
	const_iterator begin() const {return _pathEntries.begin();}; // why it should be const ??
	const_iterator end() const {return _pathEntries.end();}; // why it should be const ??

private:
	// string_type _pathname;  // exposition only
	string_vec _pathEntries;
};









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
