/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 09:29:57 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/07 13:12:53 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

# include <string>
# include <fstream>

# include "AHttpMessage.hpp"
# include "HttpStatus.hpp"

class HttpResponse: public AHttpMessage
{
	public:

	/* --- Member functions ------------------------------------------------- */

		HttpResponse();
		~HttpResponse();

		int				getStatus();
		std::string		getStatusMessage(int code);



		void			setStatus(int code);
		void			setContentLength(int contentLength);

		void			read(std::istream is);
		void			write(std::ostream os);

		void			sendError(int code, std::ofstream out);

		std::string		toString();
		

	private:

	/* --- Member variables ------------------------------------------------- */

		HttpStatus		_status;
};

#endif