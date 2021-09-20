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
	if (empty())
		return path();
	size_t pos = _path.rfind('/');
	if (pos == string_type::npos)
		return path();
	if (pos == 0)
		return path("/");
	return path(string_type(_path.begin(), _path.begin() + pos));
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

	string_type::const_iterator it = _path.begin();
	string_type::const_iterator end = _path.end();

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
	if (!_path.empty() && *--_path.end() == '/')
		result.push_back("");
	return result;
}

path path::lexically_normal() const
{
	  /*
  - If the path is empty, stop.
  - Replace each slash character in the root-name with a preferred-separator.
  - Replace each directory-separator with a preferred-separator.
  - Remove each dot filename and any immediately following directory-separator.
  - As long as any appear, remove a non-dot-dot filename immediately followed
	by a directory-separator and a dot-dot filename, along with any immediately
	following directory-separator.
  - If there is a root-directory, remove all dot-dot filenames and any
	directory-separators immediately following them.
  - If the last filename is dot-dot, remove any trailing directory-separator.
  - If the path is empty, add a dot.
  */
 	std::vector<path> p;
 	if (_path.empty())
		return path();
	path::iterator it = begin();
	path::iterator end = this->end();

	while (it != end)
	{
		std::cout << *it << "|" ;
		if (it->string() == "..")
		{
			if (p.empty() || p.size() < 1)
				p.push_back(*it);
			else
			{
				if (p.size() == 1)
				{
				 	if (p[0].is_absolute())
					{
						p.pop_back();
						p.push_back("/");
					}
					else
					{
						p.pop_back();
						p.push_back(".");
					}
				}
				else
					p.pop_back();
			
			}
		}
		else if (it->string() != ".")
			p.push_back(*it);
		else
			p.push_back("");
		++it;
	}
	{
		std::vector<path>::const_iterator it = p.begin();
		std::vector<path>::const_iterator end = p.end();

		path result;
		while (it != end)
		{
			result += it->string(); 
			result += "/";
			++it;
		}
		return result;
	}
}

path path::lexically_relative(const path& base) const
{
	path::iterator it = base.begin();
	path::iterator end = base.end();
	std::cout << *it << "|";
	
	return path();
}
path path::lexically_proximate(const path& base) const
{
	(void)base;
	return path();
}

std::ostream&	operator<<(std::ostream& os, const path& path)
{
	os << path.string();
	return os;
}


path::iterator::string_type::const_iterator
path::iterator::increment(const string_type::const_iterator& pos) const
{
	path::string_type::const_iterator i = pos;
	if (i != _last)
	{
		if (*i++ == '/')
		{
			if (i != _last && *i == '/') {
				if (!(i + 1 != _last && *(i + 1) == '/'))
				{
					// leadind double slashes detected, treat this and the
					// following until a slash as one unit
					i = std::find(++i, _last, '/');
				}
				else {
					// skip redundant slashes
					while (i != _last && *i == '/') {
						++i;
					}
				}
			}
		}
		else {
			i = std::find(i, _last, '/');
		}
	}
	return i;
}

path::iterator::string_type::const_iterator
path::iterator::decrement(const string_type::const_iterator& pos) const
{
	path::string_type::const_iterator i = pos;
	if (i != _first)
	{
		--i;
		// if this is now the root slash or the trailing slash, we are done,
		// else check for network name
		if (pos != _last || *i != '/') {
			i = std::find(std::reverse_iterator<path::string_type::const_iterator>(i),
							std::reverse_iterator<path::string_type::const_iterator>(_first), '/').base();
			// Now we have to check if this is a network name
			// if (i - _first == 2 && *_first == '/' && *(_first + 1) == '/') {
			//     i -= 2;
			// }
		}
	}
	return i;
}

void path::iterator::updateCurrent()
{
	if (_iter == _last)
	{
		_cur.clear();
		return;
	}
	if (_iter != _first && _iter != _last && *_iter == '/' && _iter + 1 == _last)
		_cur.clear();
	else
		// _cur.assign(_iter, increment(_iter));
		_cur = path(string_type(_iter, increment(_iter)));
}


path::iterator 	path::begin() const	{ return iterator(*this, _path.begin());}
path::iterator 	path::end()	const	{ return iterator(*this, _path.end());}


} /* namespace filesystem */
} /* namespace ft */
