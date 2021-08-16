/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 16:10:02 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/16 20:18:52 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utility.hpp"

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