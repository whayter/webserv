/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uri.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/05 13:06:01 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/07 11:54:47 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uri.hpp"

Uri::Uri(){}

Uri::Uri(std::string uri)
{
    std::stringstream ss(uri);
    std::getline(ss, _scheme, ':');
    _toLower(_scheme);
}

Uri::~Uri() {}

// bool isValid() {}

// bool isAbsolute() {}

// bool isOpaque() {}

std::string Uri::getScheme()
{
    return _scheme;
}

std::string Uri::getPath()
{
    return _path;
}

std::string Uri::getQueryString()
{
    return _queryString;
}

Uri::map_type Uri::getQueries(std::string queries)
{
    map_type map;
    std::string s, ss;
    size_type p, pp, count;

    count = 1 + std::count(queries.begin(), queries.end(), '&');
    for (; count; count--)
    {
        p = queries.find("&");
        ss = queries.substr(0, p);
        pp = ss.find("=");
        s = ss.substr(0, pp);
        ss.erase(0, pp + 1);
        map.insert(std::make_pair(s, ss));
        queries.erase(0, p + 1);
    }
    return map;
}

std::string Uri::decode(std::string s)
{
    std::string decoded, sub;
    std::string::iterator cursor;
    size_t pos;
    int i;

    for (cursor = s.begin(); cursor != s.end(); cursor++)
    {
        if (*cursor != '%')
            decoded.push_back(*cursor);
        else
        {
            pos = cursor - s.begin();
            sub = s.substr(pos + 1, 2);
            std::istringstream iss(sub);
            iss.flags(std::ios::hex);
            iss >> i;
            decoded.push_back(static_cast<char>(i));
            cursor += 2;
        }
    }
    return decoded;
}
