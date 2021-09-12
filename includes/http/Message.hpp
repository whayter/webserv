/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 18:53:31 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/12 23:14:19 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "http.hpp"
#include "Status.hpp"

namespace http {

class Message
{
	
public:

	Message();
	~Message();

	inline Status&			getStatus() { return _status; }
	inline headers_type		getHeaders() const { return _headers; }
	inline content_type		getContent() const { return _content; }

	std::string 			getHeader(std::string name);

	void					setStatus(Status::StatusEnum e);
	void 					setHeader(std::string key, std::string val);
	void					setContent(content_type content);

private:

	Status					_status;
	headers_type			_headers;
	content_type			_content;
};

} /* namespace http */

#endif /* MESSAGE_HPP */