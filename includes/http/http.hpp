/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 18:54:05 by hwinston          #+#    #+#             */
/*   Updated: 2021/09/12 19:23:59 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
#define HTTP_HPP

#include <string>
#include <vector>
#include <queue>
#include <map>

namespace http
{
   	/* --- Types ------------------------------------------------------------ */

    typedef std::queue<unsigned char>			queue_type;
	typedef std::deque<unsigned char>           deque_type;
	typedef std::map<std::string, std::string>	headers_type;
	typedef std::vector<unsigned char>			content_type;
		
	/* --- Classes & structures --------------------------------------------- */

	class										Message;
	// class										MessageBuilder;
	// class										Supervisor;

	struct										Status;
	
};

#endif