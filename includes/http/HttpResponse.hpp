/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 09:29:57 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/15 14:51:59 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# include "Response.hpp"
# include "HttpRequest.hpp"
# include "Status.hpp"
# include "Server.hpp"
# include "ServerConfig.hpp"

# include <string>
# include <fstream>

class HttpResponse: public http::Response
{
	public:

	/* --- Member functions ------------------------------------------------- */

		HttpResponse(ServerBlock serverBlock, HttpRequest& request);
		~HttpResponse();


		void		cgi();
		void		get();
		void		post();
		void		del();

		void		build();

		void		setLocalContent();
		void		setCgiContent();
		void		setErrorContent();

		std::string	toString();		

	private:

	/* --- Private functions ------------------------------------------------ */

		void		_parseCgiHeaders(std::vector<unsigned char>& cgiHeaders);

		std::string	_getHeader(std::string key);
		int			_getStatus(void); 
		std::string	_getStatusMessage(void);

		void		_setStatus(int code);
		void		_setStatusLine(void);
		void		_setDate(void);
		void		_setServer(void);
		void		_setContentType(const Uri& uri);
		void		_setContentLength(void);

	/* --- Member variables ------------------------------------------------- */

		ServerBlock	_serverBlock;
		HttpRequest	_request;
		std::string	_statusLine;
		std::string	_content;	
		http::Status _code;
};

#endif