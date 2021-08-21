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
# include "Uri.hpp"
# include "parser/http/ScannerHttpRequest.hpp"

class HttpRequest: public AHttpMessage
{
	public:

	/* --- Member functions ------------------------------------------------- */

		HttpRequest();
		~HttpRequest();

		/// Copy constructor
		// HttpRequest(HttpRequest &);
		/// Assignment operator
		// HttpRequest& operator=(const HttpRequest &);
		
		std::string&	getMethod();
		Uri&			getUri();
 

		void			setMethod(std::string method);
		void			setUri(const Uri &uri);
		void			setVersion(std::string version);

		std::string 	toString();

		// void			read(std::istream & inputStream);
		void			read(const char *buffer);
		void			write(std::ostream os);
		void			clear(void);
		bool			isComplete(void);

		static HttpRequest create(std::istream & inputStream);


	private:



	/* --- Member variables ------------------------------------------------- */

		// map_type		_queryParameters;
		std::string		_method, _version; // _queryString;
		Uri				_uri;

		bool	_isHeaderParsed;
		bool	_isContentParsed;

		// parser::http::ScannerHttpRequest _scanner;
		std::string _remainingChars;

};

#endif