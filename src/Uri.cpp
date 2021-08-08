/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uri.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/05 13:06:01 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/08 15:43:41 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uri.hpp"

Uri::Uri(): _port(0) {}

Uri::Uri(const std::string& uri):
    _port(0)
{
    _parseUri(uri);
}

Uri::Uri(const std::string& scheme, const std::string& pathEtc):
    _scheme(scheme), _port(0)
{
    _toLower(scheme);
    _parsePathEtc(pathEtc);
}

Uri::Uri(const std::string& scheme, const std::string& authority, const std::string& pathEtc)
    : _scheme(scheme)
{
    _toLower(scheme);
    _parseAuthority(authority);
    _parsePathEtc(pathEtc);   
}

Uri::Uri(const std::string& scheme, const std::string& authority, const std::string& path, const std::string& query)
    : _scheme(scheme), _path(path), _query(query)
{
    _toLower(scheme);
    _parseAuthority(authority);
}

Uri::Uri(const std::string& scheme, const std::string& authority, const std::string& path, const std::string& query, const std::string& fragment)
    : _scheme(scheme), _path(path), _query(query), _fragment(fragment)
{
    _toLower(scheme);
    _parseAuthority(authority);
}

Uri::Uri(Uri& other):
    _scheme(other._scheme),
    _userInfo(other._userInfo),
    _host(other._host),
    _port(other._port),
    _path(other._path),
    _query(other._query),
    _fragment(other._fragment)
{}

// Uri& operator=(const Uri& other)
// {
    
// }
// Uri& operator=(const std::string& uri)
// {
    
// }

void Uri::_parseUri(const std::string& uri){
    (void)uri;
    throw "Not implemented";
}

void Uri::_parsePathEtc(const std::string& pathEtc){
    (void)pathEtc;
    throw "Not implemented";
}


Uri::~Uri() {}


u_short					Uri::getPort() const
{
    if (_port == 0)
        return _getWellKnownPort();
    return _port;
}

std::string				Uri::getQuery() const
{
    return _query;
}

std::string				Uri::getPathEtc() const
{
    return _path + _query + _fragment;
}
std::string				Uri::getPathAndQuery() const
{
    return _path + _query;
}
std::string				Uri::getAuthority() const
{
    throw "Not implemented";
    return "";
}


void					Uri::setScheme(const std::string& scheme)
{
    _scheme = scheme;   
}
void					Uri::setUserInfo(const std::string& userInfo)
{
    _userInfo = userInfo;
}
void					Uri::setHost(const std::string& host)
{
    _host = host;
}
void					Uri::setPath(const std::string& path)
{
    _path = path;
}
void					Uri::setPathEtc(const std::string& pathEtc)
{
        throw "Not implemented";
}

void					Uri::setSpecifiedPort(u_short port)
{
    _port = port;
}
void					Uri::setRawQuery(const std::string& query)
{
    _query = query;
}

void					Uri::setPort(u_short port)
{
    _port = port;
}

void        			Uri::setQuery(const std::string& query)
{
    _query = query;
}

u_short                 Uri::_getWellKnownPort() const
{
    std::map<std::string, u_short> m;
    
    m["ftp"]    = 21;
    m["ssh"]    = 22;
    m["telnet"] = 23;
    m["http"]   = 80;
    m["ws"]     = 80;
    m["https"]  = 443;
    m["wss"]    = 443;
    
    if (m.find(_scheme) != m.end())
        return m[_scheme];
    return 0;
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
