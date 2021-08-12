/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpMessage.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 14:47:44 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/12 22:20:26 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AHTTP_MESSAGE_HPP
#define AHTTP_MESSAGE_HPP

#include <map>
#include <sstream>
#include <algorithm>

class AHttpMessage
{
	public:

	/* --- Member types ----------------------------------------------------- */

		typedef std::map<std::string, std::string>	map_type;

	/* --- Member functions ------------------------------------------------- */

		AHttpMessage();
		virtual ~AHttpMessage();

		std::string		getContent();
		std::string 	getHeader(std::string name);
		// size_t		getContentLength();
		map_type		getHeaders();


		void 			addHeader(std::string name, std::string value);
		void			setContent(std::string content);
		// void			setContentType(std::string type);
		// void 			setHeaders(map_type headers);

        // virtual void	read(std::istream is) = 0;
		void			readContent(std::istream is);
	
	protected:

	/* --- Member variables ------------------------------------------------- */

		std::string		_content;
        map_type		_headers;
};

#endif
