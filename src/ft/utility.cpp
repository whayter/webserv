/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 16:10:02 by juligonz          #+#    #+#             */
/*   Updated: 2021/09/18 11:59:12 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/utility.hpp"
#include <algorithm>

namespace ft {

std::string intToString(int i)
{
    std::string result;
    std::ostringstream ss;

	ss << i;
	result = ss.str();
	return result;
}

std::string stringifyVector(std::vector<unsigned char> v)
{
    return std::string(v.begin(), v.end());
}

std::vector<unsigned char> vectorizeString(std::string s)
{
	return std::vector<unsigned char>(s.begin(), s.end());
}

void lowerStringInPlace(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
}

void upperStringInPlace(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
}

bool isInteger(std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && ::isdigit(*it))
		++it;
    return !s.empty() && it == s.end();
}

std::string getDate()
{
	char date[30];
	time_t now = time(0);
	struct tm* lt = gmtime(&now);
	strftime(date, sizeof(date), "%a, %d %b %Y %k:%M:%S GMT", lt);
	return date;
}

std::string trim(const std::string& str,
                 const std::string& whitespace)
{
    const size_t strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return "";

    const size_t strEnd = str.find_last_not_of(whitespace);
    const size_t strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}


// Return true if both paths are equal, else false and tell how many comp are the same by variable nSameComp
bool	pathsComponentsAreEqual(const filesystem::path& one, const filesystem::path& two, size_t& nSameComp)
{
	filesystem::path::iterator itOne = one.begin();
	filesystem::path::iterator itTwo = two.begin();

	nSameComp = 0;
	for (; itOne != one.end() && itTwo != two.end(); itOne++, itTwo++)
	{
		if (*itOne != *itTwo)
		{
			if ((itOne->empty() && ++itOne == one.end())
			||	(itTwo->empty() && ++itTwo == two.end()))
				nSameComp += 1;
			return false;
		}
		nSameComp += 1;
	}
	if (itOne != one.end() || itTwo != two.end())
		return false;
	return true;
}

} /* namespace ft */
