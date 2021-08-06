/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 11:04:34 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/04 14:41:22 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST
# define HTTP_REQUEST

# include <string>

# include "AHttpMessage.hpp"
# include "Uri.hpp"

class HttpRequest: public AHttpMessage
{
	public:

	/* --- Member functions ------------------------------------------------- */

		HttpRequest();
		~HttpRequest();
		
		std::string		getMethod();
		std::string		getRequestURI();
 
		void			read(std::istream is);

		void			setMethod(std::string method);
		void			setUri(Uri &uri);
		void			setVersion(std::string version);

		std::string 	toString();

		void			write(std::ostream os);

	private:

	/* --- Member variables ------------------------------------------------- */

		map_type		_queryParameters;
		std::string		_method, _version, _queryString;
		Uri				_uri;
};

#endif