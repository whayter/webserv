/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 11:04:34 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/12 19:24:33 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST
# define HTTP_REQUEST

# include <string>

# include "AMessage.hpp"
# include "HttpStatus.hpp"
# include "Uri.hpp"
# include "parser/http/ScannerHttpRequest.hpp"

class HttpRequest: public AMessage
{
	public:

	/* --- Member functions ------------------------------------------------- */

		HttpRequest();
		~HttpRequest();
	
		inline std::string&	getMethod() { return _method; }
		inline std::string&	getVersion() { return _version; }
		inline Uri&			getUri() { return _uri; }

		void				setMethod(std::string method);
		void				setVersion(std::string version);
		void				setUri(const Uri &uri);

		void				read(const char *buffer, size_t len);

		void				clear(void);
		bool				isComplete(void);
		
		/// Return 0 if no error during request parsing
		int					getHttpErrorCode();
		size_t				getContentLength();

	private:

	/* --- Private functions ------------------------------------------------ */

		/// return false if following token is kEndOfInput.
		/// meaning the token may be incomplete
		bool 			_getCompleteToken(parser::http::Token& placeHolder, bool skipLWS = false);

	/* --- Member variables ------------------------------------------------- */

		std::string		_method, _version;
		Uri				_uri;
		Status		_code;

		bool	_isRequestLineParsed;
		bool	_isHeaderParsed;
		bool	_isComplete;

		parser::http::ScannerHttpRequest _scanner;
};

#endif