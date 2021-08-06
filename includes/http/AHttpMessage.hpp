/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpMessage.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 14:47:44 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/04 14:35:21 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AHTTP_MESSAGE_HPP
# define AHTTP_MESSAGE_HPP

# include <map>
# include <vector>
# include <sstream>
# include <algorithm>

class AHttpMessage
{
	public:

	/* --- Member types ----------------------------------------------------- */

        typedef size_t                              size_type;
		typedef std::map<std::string, std::string>	map_type;

	/* --- Member functions ------------------------------------------------- */

		AHttpMessage();
		virtual ~AHttpMessage();

		std::string		getContent();
		size_type		getContentLength();
		map_type		getHeaders();
		std::string 	getHeaderAt(std::string name);

        virtual void	read(std::istream is) = 0;
		void			readContent(std::istream is);

		void			setContent(std::string content);
		void			setContentLength(int contentLength);
		void			setContentType(std::string type);
		void 			setHeader(std::string name, std::string value);
		void 			setHeaders(map_type headers);

	protected:

	/* --- Member variables ------------------------------------------------- */

		std::string		_content;
        map_type		_headers;
};

#endif;