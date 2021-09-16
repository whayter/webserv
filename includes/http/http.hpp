/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 18:54:05 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/16 14:14:01 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
#define HTTP_HPP

#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <map>

namespace http
{
   	/* --- Types ------------------------------------------------------------ */

    typedef std::queue<unsigned char>			queue_type;
	typedef std::deque<unsigned char>           buffer_type;
	typedef std::map<std::string, std::string>	headers_type;
	typedef std::vector<unsigned char>			content_type;
		
	/* --- Classes & structures --------------------------------------------- */

	class										AMessage;
	class										Request;
	class										Response;
	struct										Status;
	class										MessageBuilder;
};

#endif