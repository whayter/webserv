/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 16:10:02 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/30 08:47:39 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utility.hpp"
#include <algorithm>

std::string intToString(int i)
{
    std::string result;
    std::ostringstream ss;

	ss << i;
	result = ss.str();
	return result;
}

void lowerStringInPlace(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), tolower);
}

bool isInteger(std::string& s)
{
	std::string::const_iterator it = s.begin();
	std::string::const_iterator end = s.end();
	while (it != end)
	{
		if (!isdigit(*it))
			return true;
		it++;
	}
	return true;
}

std::string getDate()
{
	char date[30];
	time_t now = time(0);
	struct tm* lt = gmtime(&now);
	strftime(date, sizeof(date), "%a, %d %b %Y %k:%M:%S GMT", lt);
	return date;
}