/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uri.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/05 13:06:01 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/09 11:57:23 by juligonz         ###   ########.fr       */
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
    _lowerStringInPlace(_scheme);
    std::string::const_iterator it = pathEtc.cbegin();
    std::string::const_iterator end = pathEtc.cend();
    _parsePathEtc(it, end);   
}

Uri::Uri(const std::string& scheme, const std::string& authority, const std::string& pathEtc)
    : _scheme(scheme)
{
    _lowerStringInPlace(_scheme);
    std::string::const_iterator it = authority.cbegin();
    std::string::const_iterator end = authority.cend();
    _parseAuthority(it, end);
    it = pathEtc.cbegin();
    end = pathEtc.cend();
    _parsePathEtc(it, end);   
}

Uri::Uri(const std::string& scheme, const std::string& authority, const std::string& path, const std::string& query)
    : _scheme(scheme), _path(path), _query(query)
{
    _lowerStringInPlace(_scheme);
    std::string::const_iterator it = authority.cbegin();
    std::string::const_iterator end = authority.cend();
    _parseAuthority(it, end);
}

Uri::Uri(const std::string& scheme, const std::string& authority, const std::string& path, const std::string& query, const std::string& fragment)
    : _scheme(scheme), _path(path), _query(query), _fragment(fragment)
{
    _lowerStringInPlace(_scheme);
    std::string::const_iterator it = authority.cbegin();
    std::string::const_iterator end = authority.cend();
    _parseAuthority(it, end);
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

Uri& Uri::operator=(const Uri& other)
{
    if (this == &other)
        return *this;
    _scheme = other._scheme;
    _userInfo = other._userInfo;
    _host = other._host;
    _port = other._port;
    _path = other._path;
    _query = other._query;
    _fragment = other._fragment;
    return *this;
}
Uri& Uri::operator=(const std::string& uri)
{
    clear();
    _parseUri(uri);
    return *this;
}

void Uri::_parseUri(const std::string& uri){
    std::string scheme;
    std::string::const_iterator it = uri.cbegin();
    std::string::const_iterator end = uri.cend();

    if (it == end)
        return;
    if (*it != '.' && *it != '/' && *it != '?' && *it != '#')
    {
        while(it != end && *it != ':' && *it != '/' && *it != '?' && *it != '#')
            scheme += *it++;
        if (it != end && *it == ':')
        {
            it++;
            if (it == end)
                throw SyntaxError();
            _scheme = scheme;
            _lowerStringInPlace(_scheme);
            if (*it == '/')
            {
                it++;
                if (it != end && *it == '/')
                {
                    it++;
                    _parseAuthority(it, end);
                }
                else
                    it--;
            }
            _parsePathEtc(it,end);
        }
        else
        {
            it = uri.begin();
            _parsePathEtc(it, end);
        }
    }
    else
        _parsePathEtc(it, end);
}

//       authority   = [ userinfo "@" ] host [ ":" port ]
void Uri::_parseAuthority(std::string::const_iterator& it, const std::string::const_iterator& end)
{
    std::string tmp;

    while(it != end && *it != '/' && *it != '?' && *it != '#')
    {
        if (*it == '@')
        {   
            _userInfo = tmp;
            tmp.clear();
        }
        else
            tmp += *it;
        it++;
    }
    std::string::const_iterator ite = tmp.cbegin();
    std::string::const_iterator ende = tmp.cend();
    _parseHostAndPort(ite, ende);
}

void Uri::_parsePathEtc(std::string::const_iterator& it, const std::string::const_iterator& end)
{
    std::string path;
    std::string query;
    std::string fragment;
    bool isFragment = false;
  
    while (it != end && *it != '?' && *it != '#')
    {
        path += *it;
        it++;
    }
    if (*it == '#')
        isFragment = true;
    if (it != end)
        it++;
    while (it != end && *it != '#' && !isFragment)
        {
            query += *it;
            it++;
        }
    if (it != end && !isFragment)
        it++;
    while (it != end)
    {
        fragment += *it;
        it++;
    }
    _path = path;
    _query = query; 
    _fragment = fragment;
}

void Uri::_parseHostAndPort(std::string::const_iterator& it, const std::string::const_iterator & end)
{
    std::string host;
    u_short port = 0;

    while(it != end && *it != ':' && *it != '/')
        host += *it++;
    
    if (it != end && *it == ':')
    {
        it++;
        while (it != end && isdigit(*it))
        {
            port = port * 10 + *it - '0';
            it++;
        }
    }
    _host =  host;
    _lowerStringInPlace(_host);
    _port = port;
}

Uri::~Uri() {}

u_short					Uri::getPort() const
{
    if (_port == 0)
        return getWellKnownPort();
    return _port;
}

std::string				Uri::getQuery() const
{
    return decode(_query);
}

std::string				Uri::getPathEtc() const
{
   std::string result;

    result += _path;
    if (!_query.empty())
        result += '?' + _query;
    if (!_fragment.empty())
        result += '#' + _fragment;
    return result;
}
std::string				Uri::getPathAndQuery() const
{
    if (!_query.empty())
        return _path + '?' + _query;
    return _path;
}
std::string				Uri::getAuthority() const
{
    std::string result;

    if (!_userInfo.empty())
        result += _userInfo + "@";
    result += _host;
    if (_port != 0 && !isWellKnownPort())
        result += ':' + std::to_string(_port);
    return result;
}

std::string		Uri::getFragment()
{
    return decode(_fragment);
}

void					Uri::setScheme(const std::string& scheme)
{
    _scheme = scheme;
    _lowerStringInPlace(_scheme);
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
    _path = decode(path);   
}
void					Uri::setPathEtc(const std::string& pathEtc)
{
    (void)pathEtc;
    _path.clear();
    _query.clear();
    _fragment.clear();
    std::string::const_iterator it = pathEtc.cbegin();
    std::string::const_iterator end = pathEtc.cend();
    _parsePathEtc(it ,end);
}

void					Uri::setSpecifiedPort(u_short port)
{
    _port = port;
}
void					Uri::setRawQuery(const std::string& query)
{
    _query = query;
}
void					Uri::setFragment(const std::string& fragment)
{
    _fragment = decode(fragment);
}

void					Uri::setPort(u_short port)
{
    _port = port;
}

void        			Uri::setQuery(const std::string& query)
{
    _query = query;
}

void					Uri::setAuthority(const std::string& authority)
{
    _userInfo.clear();
    _host.clear();
    _port = 0;
    std::string::const_iterator it = authority.cbegin();
    std::string::const_iterator end = authority.cend();
    _parseAuthority(it, end);
}


u_short                 Uri::getWellKnownPort() const
{
    std::map<std::string, u_short> m;
    
    m["ftp"]    = 21;
    m["ssh"]    = 22;
    m["telnet"] = 23;
    m["http"]   = 80;
    m["ws"]     = 80;
    m["https"]  = 443;
    m["wss"]    = 443;
    m["ldap"]    = 389;
    
    if (m.find(_scheme) != m.end())
        return m[_scheme];
    return 0;
}

bool Uri::isWellKnownPort() const
{
    return _port == getWellKnownPort();
}

bool Uri::isRelative() const
{
    return _scheme.empty();
}

bool Uri::empty() const
{
    return _scheme.empty() && _userInfo.empty() && _port == 0
        && _path.empty() && _query.empty() && _fragment.empty()
    ;
}

std::string Uri::decode(std::string s) const
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

// https://en.wikipedia.org/wiki/File:URI_syntax_diagram.svg

std::string			Uri::toString()
{
    std::string result;

    if (isRelative())
    {
        result = _path;
        if (!_query.empty())
            result += '?' + _query;
        if (!_fragment.empty())
            result += '#' + _fragment;
    }
    else
    {
        result += _scheme + ':';
        if (!_host.empty() || _scheme == "file")
            result += "//";
        result += getAuthority();
        if (!_query.empty() && _path[0] != '/')
                result += "/";
        result += getPathEtc();
    }
    return result;
}


void Uri::clear()
{
    _scheme.clear();
    _userInfo.clear();
    _host.clear();
    _port = 0;
    _path.clear();
    _query.clear();
    _fragment.clear();
}

void Uri::_lowerStringInPlace(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), tolower);
}