/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 09:29:57 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/01 00:18:30 by hwinston         ###   ########.fr       */
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
	/* --- Member functions ------------------------------------------------- */

	public:

	    /* --- Constructor & destructor ------------------------------------- */

		HttpResponse()
		{
			_status.setValue(HttpStatus::OK);
		}
		
		~HttpResponse() {}

	    /* --- Getters ------------------------------------------------------ */

		int getStatus()
		{
			return _status.getValue();
		}
		
		std::string getStatusMessage(int code)
		{
			return _status.getMessage(code);
		}






		void read(bytes_type content)
		{
			std::string line;
			std::vector<std::string> tokens;

			try
			{
				line = in.readHttpLine();
				tokens = line.split("\\s");
				try
				{
					this->setStatus(Integer.parseInt(tokens[1]));
				}
				catch (NumberFormatException nfe)
				{
					setStatus(-1);          
				}
				this->setHeader(line, ":");
			}
			catch (IOException ioe)
			{
				setStatus(SC_NO_RESPONSE);          
			}
		}







		void sendError(int code, std::ofstream out)
		{
			out << "<html><body><p>HTTP Error ";
			out << code << " - " << this->getStatusMessage(code);
			out << "</p></body></html>";

			setStatus(code);
		}

	    /* --- Setters ------------------------------------------------------ */

		void setStatus(int code)
		{
			_status.setValue(code);
		}

	/* --- Member variables ------------------------------------------------- */

	private:

		HttpStatus _status;
};

#endif