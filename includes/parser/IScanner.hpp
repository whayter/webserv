/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IScanner.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/13 18:01:37 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/21 15:19:04 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISCANNER_HPP
#define ISCANNER_HPP

namespace parser {


class IScanner
{
public:
	virtual ~IScanner(){};

	virtual char	peek(void)		= 0;
	virtual int		getLine(void)	= 0;
	virtual int		getColumn(void)	= 0;
	virtual bool	eof(void)		= 0;
	
	virtual char	get() = 0;
	virtual char	putback(char c) = 0;

}; /* Interface IScanner */

} /* namespace parser */

#endif /* ISCANNER_HPP */
