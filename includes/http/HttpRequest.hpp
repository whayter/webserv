/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 11:04:34 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/21 19:22:38 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST
# define HTTP_REQUEST

# include <string>

# include "AHttpMessage.hpp"
# include "HttpStatus.hpp"
# include "Uri.hpp"
# include "parser/http/ScannerHttpRequest.hpp"

class HttpRequest: public AHttpMessage
{
	public:

	/* --- Member functions ------------------------------------------------- */

		HttpRequest();
		~HttpRequest();
	
		std::string&	getMethod();
		Uri&			getUri();
 

		void			setMethod(std::string method);
		void			setUri(const Uri &uri);
		void			setVersion(std::string version);

		std::string 	toString();

		void			read(const char *buffer);
		void			write(std::ostream os);

		void			clear(void);
		bool			isComplete(void);
		
		/// Return 0 if no error during request parsing
		int				getHttpErrorCode();

	private:
		/// return false if following token is kEndOfInput.
		/// meaning the token may be incomplete
		bool 			_getCompleteToken(parser::http::Token& placeHolder, bool skipLWS = false);

	/* --- Member variables ------------------------------------------------- */

		std::string		_method, _version;
		Uri				_uri;
		HttpStatus		_code;

		bool	_isRequestLineParsed;
		bool	_isHeaderParsed;
		bool	_isComplete;

		parser::http::ScannerHttpRequest _scanner;
};

#endif