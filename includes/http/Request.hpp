/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/15 14:43:29 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/15 14:53:55 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "http.hpp"
#include "AMessage.hpp"
#include "Uri.hpp"

namespace http {

class Request: public AMessage {

public:

	Request();
	~Request();

	inline std::string		getMethod() { return _method; }
	inline Uri&				getUri() { return _uri; }

	void					setMethod(std::string method);
	void					setUri(const Uri& uri);

	bool					empty() const;
	void					clear();

private:

	std::string				_method;
	Uri						_uri;

}; /* class Request */

}; /* namespace http */

#endif /* REQUEST_HPP */