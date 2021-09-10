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

#include "ft/filesystem/path.hpp"

namespace ft{ 
namespace filesystem{ 
path::path() throw() {}

path::path(const path& other):
	_path(other._path)
{}

path::path(const string_type& path):
	_path(path)
{
	_formatPathInPlace();
}
path::path(const char* path):
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

path& path::operator=(const string_type& path)
{
	_path = path;
	_formatPathInPlace();
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
	if (p.empty())
		_path += '/';
	else if (p.is_absolute())
		_path = p._path;
	else if ( p.is_relative())
	{
		_path += '/';
		_path += p._path;
	}
	else
		_path += p._path;
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
	if (is_absolute() && _path.size() > 1)
		return path(string_type(_path.begin()+1, _path.end()));
	if (is_relative())
		return *this;
	return path();
	// return empty() ? path() : path(*--end());
}
path path::parent_path() const {
	throw "Flemme";
	// return (empty() || begin() == --end()) ? path() : path(*this);
}
path path::filename() const {
	if (empty())
		return path();
	size_t pos = _path.rfind('/');
	if (pos != string_type::npos)
		return path(string_type(_path.begin() + pos + 1, _path.end()));
	return *this;
	// return empty() ? path() : path(*--end());
}
path path::stem() const
{
	size_t pos = _path.rfind('/');
	
	string_type filename(_path.begin() + pos + 1, _path.end());
	if (filename == "." || filename == "..")
		return path(filename);
	pos = filename.rfind('.');
	if (pos < 1 || pos == string_type::npos)
		return path(filename);
	return path(string_type(filename.begin(), filename.begin() + pos));
}
path path::extension()	const 
{
	size_t pos = _path.rfind('.');
	if (pos != string_type::npos && _path.rfind('/') + 1 != pos)
		return path(string_type(_path.begin() + pos, _path.end()));
	return path();
}

void  path::clear() throw()
{
	_path.clear();
}

path& path::remove_filename()
{
	_path.erase(_path.rfind('/')+1);
	return *this;
}
path& path::replace_filename(const path& replacement)
{
	remove_filename();
	_path += replacement;
	return *this;
}

path& path::replace_extension(const path& replacement)
{
	size_t pos = _path.rfind('.');
	if (pos != string_type::npos)
		_path.erase(_path.rfind('.'));
	if (replacement.empty())
		return *this;
	if (replacement._path.find('.') == string_type::npos)
		_path += '.';
	_path += replacement._path;
	return *this;
}

void  path::swap(path& rhs) throw()
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


} /* namespace filesystem */
} /* namespace ft */
