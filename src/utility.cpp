/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 16:10:02 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/16 16:10:30 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utility.hpp"

std::string IntToString(int i)
{
    std::string result;
    std::ostringstream ss;

	ss << i;
	result = ss.str();
	
}