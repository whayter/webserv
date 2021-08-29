/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 09:29:57 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/29 15:44:04 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# include "AHttpMessage.hpp"
# include "HttpRequest.hpp"
# include "HttpStatus.hpp"
# include "ServerHandler.hpp"

# include <string>
# include <fstream>

class HttpResponse: public AHttpMessage
{
	public:

	/* --- Member functions ------------------------------------------------- */

		HttpResponse(server::Server server, HttpRequest& request);
		~HttpResponse();

		void			setMandatory();

		void			setContent();
		void			setError(int code);

		void methodGet(std::string filename);

		std::string		toString();		

	private:

	/* --- Private functions ------------------------------------------------ */

		std::string		_getHeader(std::string key);
		int				_getStatus(void); 
		std::string		_getStatusMessage(void);

		void			_setStatus(int code);
		void			_setStatusLine(void);
		void			_setDate(void);
		void			_setServer(void);
		void			_setContentType(const Uri& uri);
		void			_setContentLength(int contentLength);

	/* --- Member variables ------------------------------------------------- */

		server::Server	_server;
		HttpRequest		_request;
		std::string		_statusLine;
		std::string		_content;	
		HttpStatus		_code;
};

#endif