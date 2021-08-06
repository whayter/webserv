/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 09:29:57 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/04 14:51:20 by hwinston         ###   ########.fr       */
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

		void			read(std::istream is);

		void			sendError(int code, std::ofstream out);

		void			setStatus(int code);

		std::string		toString();
		
		void			write(std::ostream os);

	private:

	/* --- Member variables ------------------------------------------------- */

		HttpStatus		_status;
};

#endif