/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 18:53:31 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/15 14:46:05 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMESSAGE_HPP
#define AMESSAGE_HPP

#include "http.hpp"

namespace http {

class AMessage {
	
public:

	AMessage();
	virtual ~AMessage() {}
	

	inline headers_type&	getHeaders() { return _headers; }
	inline content_type&	getContent() { return _content; }
	inline std::string		getVersion() { return _version; }

	std::string 			getHeader(std::string name);
	size_t 					getContentLength();

	void 					setHeader(std::string key, std::string val);
	void					setContent(const content_type &content);
	void					setVersion(const std::string &version);

	virtual bool			empty() const = 0;
	virtual void			clear() = 0;

protected:

	headers_type			_headers;
	content_type			_content;
	std::string				_version;

}; /* class Message */

} /* namespace http */

#endif /* MESSAGE_HPP */