/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IUri.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 10:57:01 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/03 11:10:23 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IURI_HPP
#define IURI_HPP

#include <string>

class IUri
{
public:
	virtual ~IUri();
	virtual bool isValid() = 0;
	virtual bool isAbsolute() = 0;
	virtual bool isOpaque() = 0;
	virtual std::string scheme() = 0;
	virtual std::string path() = 0;
	virtual std::string query() = 0;

	// virtual bool operator== ( const IURI& obj ) const = 0;
};

IUri::IUri(/* args */)
{
}

IUri::~IUri()
{
}

#endif
