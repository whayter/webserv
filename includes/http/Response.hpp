/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/15 14:45:08 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/25 15:55:30 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "http.hpp"
#include "Message.hpp"
#include "Status.hpp"

namespace http {

class Response: public Message {

public:

	Response();
	~Response();

	inline Status&			getStatus() { return _status; }
	
	void					setStatus(Status e);

	bool					empty() const;
	void					clear();

private:

	Status					_status;

}; /* class Response */

}; /* namespace http */

#endif /* REPSONSE_HPP */