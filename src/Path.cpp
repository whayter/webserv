/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Path.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/05 13:06:01 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/16 18:01:36 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Path.hpp"

Path::Path(){}

Path::Path(const Path& other):
	_path(other._path)
{}

Path::Path(const string_type& path):
	_path(path)
{
}

Path::~Path() {}


Path& Path::operator=(const Path& other)
{
	if (this == &other)
		return *this;
	_path = other._path;
	return *this;
}

Path& Path::operator/=(const Path& p)
{
	(void)p;
	return *this;
}

Path Path::root_directory()	const { 
	return !_path.empty() && _path[0] == '/' ? Path("/") : Path() ;
	// return !_path.empty() && _path[0] == '/' ? Path(_pathEntries[0]) : Path() ;
}
Path Path::root_path() const {
	return /* root_name() / */ root_directory();
}
Path Path::relative_path() const {
	return Path();
	// return empty() ? Path() : Path(*--end());
}
Path Path::parent_path() const {
	return Path();
	// return (empty() || begin() == --end()) ? Path() : Path(*this);
}
Path Path::filename() const {
	return Path();
	// return empty() ? Path() : Path(*--end());
}
Path Path::stem() const
{
	if (empty())
		return Path();
		return Path();
	// std::string filename = *--end();
	// if (filename == "." || filename == "..")
	// 	return Path();
	// return Path(filename.substr(0, filename.rfind('.')));
}
Path Path::extension()	const 
{
	if (empty())
		return Path();
		return Path();
	// std::string filename = *--end();
	// if (filename == "." || filename == "..")
	// 	return Path();
	// return Path(filename.substr(filename.rfind('.')));
}





///////////////////////////////////////////////////
///////////////////////////////////////////////////

// Path::Path(): _absolute(false) {}

// Path::Path(bool absolute): _absolute(absolute) {}

// Path::Path(const std::string& path)
// {
// 	parse(path);
// }
// Path::Path(const Path& other):
// 	// _path(other._path),
// 	_dirs(other._dirs),
// 	_absolute(other._absolute)
// {}

// Path::~Path() {}


// Path& Path::operator=(const Path& other)
// {
// 	if (this == &other)
// 		return *this;
// 	_absolute = other._absolute;
// 	// _path = other._path;
// 	return *this;
// }

// Path& Path::operator=(const std::string& path)
// {
// 	clear();
// 	parse(path);
// 	return *this;
// }

// void	Path::clear()
// {
// 	_absolute = false;
// 	// _path.clear();
// 	_dirs.clear();
// }

// std::string Path::current()
// {
// 	std::string result;
// 	char cwd[PATH_MAX];
// 	if (!getcwd(cwd, sizeof(cwd)))
// 		throw std::runtime_error("getcwd() failed");
// 	result = cwd;
// 	return result;
// }

// std::string Path::home()
// {
// 	std::string result;
// 	struct passwd* pwd= getpwuid(getuid());
// 	if (pwd)
// 		result = pwd->pw_dir;
// 	else
// 		throw std::runtime_error("getpwuid() failed");
// 	return result;
// }

// void Path::parse(const std::string& path)
// {
// 	this->clear();

// 	std::string::const_iterator it = path.begin();
// 	std::string::const_iterator end = path.end();

// 	if (it == end)
// 		return;
// 	switch (*it)
// 	{
// 	case '/':
// 		{
// 			_absolute = true;
// 			++it;
// 		}
// 	break;
// 	case '~':
// 		{
// 			++it;
// 			if (it != end && *it != '/' )
// 				--it;
// 			else
// 			{
// 				Path cwd(home());
// 				_dirs = cwd._dirs;
// 				_absolute = true;
// 			}
// 		}
// 	break;
// 	default:
// 	break;
// 	}
// 	while (it != end)
// 	{
// 		std::string s;
// 		while (it != end && *it != '/')
// 			s += *it++;
// 		if (it == end)
// 			_filename= s;
// 		else
// 		{
// 			_dirs.push_back(s);
// 			++it;
// 		}
// 	}
// }

// std::string Path::toString() const
// {
// 	std::string result;
// 	std::vector<std::string>::const_iterator it = _dirs.begin();
// 	std::vector<std::string>::const_iterator end = _dirs.end();

// 	if (_absolute)
// 		result += '/';
// 	while (it != end)
// 	{
// 		result += *it;
// 		result += '/';
// 	}
// 	result += _filename;
// 	return result;
// }
