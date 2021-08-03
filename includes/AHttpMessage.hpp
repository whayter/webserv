/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpMessage.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 14:47:44 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/01 00:27:03 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AHTTP_MESSAGE_HPP
# define AHTTP_MESSAGE_HPP

# include <map>
# include <vector>
# include <sstream>
# include <fstream>

class AHttpMessage
{
	public:

	/* --- Member types ----------------------------------------------------- */

        typedef size_t                              size_type;
        typedef std::vector<unsigned char>          bytes_type;
		typedef std::map<std::string, std::string>	headers_type;

	/* --- Member functions ------------------------------------------------- */

	    /* --- Constructor & destructor ------------------------------------- */

		AHttpMessage(): _content(NULL), _headers() {}
		
		virtual ~AHttpMessage() {}

	    /* --- Getters ------------------------------------------------------ */

		bytes_type getContent()
		{
			return _content;
		}

		size_type getContentLength()
		{
            size_type result;
			try
			{
                std::istringstream(getHeader("Content-Length")) >> result;
			}
			catch(const std::exception& e)
			{
				result = 0;
			}
			return result;
		}
 
		std::string getHeader(std::string name)
		{
			return _headers.at(name);
		}
 
		headers_type::iterator getHeaderNames()
		{
			return _headers.begin();
		}

        virtual void read(std::ifstream ifs)
		{
			// throws IndexOutOfBoundsException, IOException, URISyntaxException; // ???
		}

		void readContent(bytes_type content)
		{
			size_t contentLength;
            contentLength = this->getContentLength();
			if (contentLength > 0)
				_content = content;
		}

	    /* --- Setters ------------------------------------------------------ */

		void setContent(bytes_type content)
		{
            size_t i = 0;
            while (content[i])
                i++;
			this->setContentLength(i);
            _content = content;
		}

		void setContentLength(int contentLength)
		{
			std::ostringstream os;
			os << contentLength;
			this->setHeader("Content-Length", os.str());
		}

		void setContentType(std::string type)
		{
			this->setHeader("Content-Type", type);       
		}

		void setHeader(std::string name, std::string value)
		{
			_headers.insert(std::make_pair<std::string, std::string>(name, value));
		}

		void setHeaders(headers_type headers)
		{
			_headers = headers;
		}

	protected:
	
	/* --- Member variables ------------------------------------------------- */
	
        bytes_type                              _content;
        headers_type							_headers;	
};

#endif;