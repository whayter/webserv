/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 14:47:44 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/08 02:33:55 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMESSAGE_HPP
#define AMESSAGE_HPP

#include <vector>
#include <map>
#include <string>

class AMessage
{
	public:

	/* --- Member types ----------------------------------------------------- */

		typedef std::map<std::string, std::string>	headers_type;
		typedef std::vector<unsigned char>			content_type;

	/* --- Member functions ------------------------------------------------- */

		AMessage();
		virtual ~AMessage();

		inline headers_type		getHeaders() { return _headers; }
		inline content_type		getContent() { return _content; }

		/// returns a reference to the value associated with name.
		/// returns an empty string if name is not a valid header.
		std::string& 			getHeader(std::string name);

		void 					setHeader(std::string name, std::string value);
		void					setContent(content_type content);

	protected:

	/* --- Member variables ------------------------------------------------- */

		content_type		_content;
        headers_type		_headers;
};

#endif
