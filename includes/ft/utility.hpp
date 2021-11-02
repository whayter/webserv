/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 16:06:58 by juligonz          #+#    #+#             */
/*   Updated: 2021/11/01 14:46:32 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include "filesystem.h"

namespace ft {

void 						writeContentToFile(const filesystem::path& path, const char *content, size_t n);
std::vector<unsigned char>	getFileContent(const filesystem::path& path);
std::string					intToString(int i);
std::string					stringifyVector(std::vector<unsigned char> v);
std::vector<unsigned char>	vectorizeString(std::string s);
void						lowerStringInPlace(std::string& s);
void						upperStringInPlace(std::string& s);
bool						isInteger(std::string& s);
std::string					getDate();
std::string					trim(const std::string& str,
								 const std::string& whitespace = " \t");
std::vector<std::string> 	split(const std::string& str, char delim);

/// return the number of consecutives same elements in two paths.
bool	pathsComponentsAreEqual(const filesystem::path& one, const filesystem::path& two, size_t& nSameComp);

bool						isValidIpAddress(const char *ipAddress);

} /* namespace ft */

#endif
