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

namespace ft{ 
namespace filesystem{ 
path::path(){}

path::path(const path& other):
	_path(other._path)
{}

path::path(const string_type& path):
	_path(path)
{
	_formatPathInPlace();
}

path::~path() {}


path& path::operator=(const path& other)
{
	if (this == &other)
		return *this;
	_path = other._path;
	return *this;
}

// template <class Source>
// path& path::operator=(const Source& source)
// {
// 	_path = source;
// 	return *this;
// }

path& path::operator/=(const path& p)
{
	std::cout << _path <<"||"<< p._path << "|" << std::endl;
	if (p.empty())
		_path += '/';
	else if (p.is_absolute())
		_path = p._path;
	else
		_path += p._path;
	std::cout << _path << std::endl;
	// _formatPathInPlace();

	return *this;
}

path path::root_directory()	const { 
	return !_path.empty() && _path[0] == '/' ? path("/") : path() ;
	// return !_path.empty() && _path[0] == '/' ? path(_pathEntries[0]) : path() ;
}
path path::root_path() const {
	return /* root_name() / */ root_directory();
}
path path::relative_path() const {
	return path();
	// return empty() ? path() : path(*--end());
}
path path::parent_path() const {
	return path();
	// return (empty() || begin() == --end()) ? path() : path(*this);
}
path path::filename() const {
	return path();
	// return empty() ? path() : path(*--end());
}
path path::stem() const
{
	if (empty())
		return path();
		return path();
	// std::string filename = *--end();
	// if (filename == "." || filename == "..")
	// 	return path();
	// return path(filename.substr(0, filename.rfind('.')));
}
path path::extension()	const 
{
	if (empty())
		return path();
		return path();
	// std::string filename = *--end();
	// if (filename == "." || filename == "..")
	// 	return path();
	// return path(filename.substr(filename.rfind('.')));
}

void  path::clear()
{
	_path.empty();
}
path& path::remove_filename()
{

	return *this;
}
path& path::replace_filename(const path& replacement)
{
	(void)replacement;
	return *this;
}
path& path::replace_extension(const path& replacement)
{
	(void)replacement;
	return *this;
}

void  path::swap(path& rhs)
{
	path tmp(*this);

	*this = rhs;
	rhs = tmp;
}

void path::_formatPathInPlace()
{
	std::vector<string_type> pathEntries = _splitPath();
	std::vector<string_type>::const_iterator it = pathEntries.begin();
	std::vector<string_type>::const_iterator end = pathEntries.end();
	string_type result;

	if (pathEntries.size() > 2 && pathEntries[0].empty() && pathEntries[1].empty() && !pathEntries[2].empty())
	{
		result += "//";
		it += 2;
	}
	while (it != end)
	{
		if (it->empty() && it != pathEntries.begin())
		{
			++it;
			continue;
		}
		if (!it->empty())
			result += *it;
		if (it + 1 != end)
			result += '/';
		it++;
	}
	_path = result;
}

std::vector<path::string_type> path::_splitPath()
{
	std::vector<string_type> result;

	string_type::const_iterator it = begin();
	string_type::const_iterator end = this->end();

	string_type tmp;
	while (it != end)
	{
		if (*it == '/')
		{
			result.push_back(tmp);
			tmp.clear();
		}
		else
			tmp += *it;
		it++;
	}
	if (!tmp.empty())
		result.push_back(tmp);
	if (!_path.empty() && *--this->end() == '/')
		result.push_back("");
	return result;
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

} /* namespace filesystem */
} /* namespace ft */
