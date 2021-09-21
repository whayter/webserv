/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 16:06:58 by juligonz          #+#    #+#             */
/*   Updated: 2021/09/18 11:59:28 by hwinston         ###   ########.fr       */
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

std::string					intToString(int i);
int							stringifyInteger(std::string integer);
std::string					stringifyVector(std::vector<unsigned char> v);
std::vector<unsigned char>	vectorizeString(std::string s);
void						lowerStringInPlace(std::string& s);
bool						isInteger(std::string& s);
std::string					getDate();

/// return the number of consecutives same elements in two paths.
bool	pathsComponentsAreEqual(const filesystem::path& one, const filesystem::path& two, size_t& nSameComp);

} /* namespace ft */

#endif
