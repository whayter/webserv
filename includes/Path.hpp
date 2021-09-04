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

#include "SyntaxError.hpp"

// #include <limits.h> for linux

#define PATH_MAX 1024 // For linux, don't care for now. Override macos define (but same len)

// https://man7.org/linux/man-pages/man7/path_resolution.7.html
// https://insanecoding.blogspot.com/2007/11/pathmax-simply-isnt.html
// https://www.gnu.org/software/libc/manual/html_node/Limits-for-Files.html


class Path
{
public:

	/// @brief Create empty Path
	Path();
	/// @brief Creates an empty absolute or relative path.
	Path(bool absolute);
	/// @brief Create Path from string
	Path(const std::string& path);
	/// @brief Copy constructor
	Path(const Path&);
	/// @brief Assignment operator
	Path& operator=(const Path&);
	/// @brief Parse string and assign it
	Path& operator=(const std::string& path);

	~Path();


	// /// Returns the filename.
	// std::string		getFilename() const;
	// /// Returns the basename (filename without extension)
	// std::string		getBaseName() const;
	// /// Returns the filename extension.
	// std::string		getExtention() const;


	/// Returns true if the path refer to a directory (the filename part is empty).
	inline bool		isDirectory() const { return _filename.empty();}
	/// Returns true if the path refer to a file (the filename part is not empty).
	inline bool		isFile() const {return !_filename.empty();}
	/// Returns true if the path is absolute.
	inline bool		isAbsolute() const { return _absolute;}
	/// Returns true if the path is relative.
	inline bool		isRelative() const { return !_absolute;}


	inline size_t	depth() const { return _dirs.size();}


	/// Makes the path absolute if it is relative. The current working directory is taken as base directory.
	Path & makeAbsolute();


	/// Returns the user's home directory.
	static std::string home();
	/// Returns the current working directory.
	static std::string current();
	/// Returns the temporary directory.
	static std::string temp();


	/// Returns an absolute variant of the path, taking the current working directory as base.
	Path absolute() const;
	/// Returns an absolute variant of the path, taking the given path as base.
	Path absolute(const Path & base) const;


	void	clear();
	std::string toString() const;

	void parse(const std::string& path);

private:

	std::string					_filename;
	std::vector<std::string>	_dirs; // list of previous directories
	bool						_absolute;
};

#endif /* PATH_HPP */
