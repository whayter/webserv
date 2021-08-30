/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/16 16:06:58 by juligonz          #+#    #+#             */
/*   Updated: 2021/08/30 13:08:37 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>
#include <sstream>
#include <ctime>

std::string intToString(int i);
void		lowerStringInPlace(std::string& s);
bool		isInteger(std::string& s);
std::string getDate();

#endif
