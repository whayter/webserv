/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uri.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 13:01:23 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/07 11:00:21 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URI_HPP
# define URI_HPP

# include <string>
# include <sstream>
# include <cctype>
# include <algorithm>
# include <map>

class Uri
{
	public:

	/* --- Member types ----------------------------------------------------- */

		typedef std::map<std::string, std::string>	map_type;
		typedef size_t								size_type;

	/* --- Member functions ------------------------------------------------- */

		Uri(std::string uri);
		~Uri();

		bool			isValid();
		bool			isAbsolute();
		bool			isOpaque();

		std::string		getScheme();
		std::string		getPath();
		std::string		getQueryString();
		map_type		getQueries(std::string queries);
		
		std::string		decode(std::string s);

		//bool operator==(const Uri& obj) const {}

	private:

	/* --- Member variables ------------------------------------------------- */

		std::string _scheme, _path, _queryString;

	/* --- Private functions ------------------------------------------------ */

		std::string _toLower(std::string s)
		{
			std::transform(s.begin(), s.end(), s.begin(), tolower);
			return s;
		}
};

#endif