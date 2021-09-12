/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 18:53:31 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/12 19:17:47 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "http.hpp"

namespace http
{
    class Message
    {
        public:

        /* --- Member functions --------------------------------------------- */

            Message();
            ~Message();
    
            int					getStatusCode() { return _statusCode; }
            std::string			getStatusMessage() const { return Status::getDefinition(_statusCode); }
            headers_type		getHeaders() const { return _headers; }
            content_type		getContent() const { return _content; }

            std::string 		getHeader(std::string name);

            void				setStatusCode(int code);
            void 				setHeader(std::string key, std::string val);
            void				setContent(content_type content);

        private:

        /* --- Member variables --------------------------------------------- */

            Status					_statusCode;
            content_type			_content;
            headers_type			_headers;
    };
}

#endif