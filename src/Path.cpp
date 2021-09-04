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

Path::Path(): _absolute(false) {}

Path::Path(bool absolute): _absolute(absolute) {}

Path::Path(const std::string& path)
{
	parse(path);
}
Path::Path(const Path& other):
	_path(other._path),
	_dirs(other._dirs),
	_absolute(other._absolute)
{}

Path::~Path() {}


Path& Path::operator=(const Path& other)
{
	if (this == &other)
		return *this;
	_absolute = other._absolute;
	_path = other._path;
	return *this;
}

Path& Path::operator=(const std::string& path)
{
	clear();
	parse(path);
	return *this;
}

void	Path::clear()
{
	_absolute = false;
	_path.clear();
	_dirs.clear();
}

std::string Path::current()
{
	std::string result;
	char cwd[PATH_MAX];
	if (!getcwd(cwd, sizeof(cwd)))
		throw std::runtime_error("getcwd() failed");
	result = cwd;
	return result;
}

std::string Path::home()
{
	std::string result;
	char cwd[PATH_MAX];
	if (!getcwd(cwd, sizeof(cwd)))
		throw std::runtime_error("getcwd() failed");
	result = cwd;
	return result;
}



void Path::parse(const std::string& path)
{
	this->clear();
	(void)path;
}


