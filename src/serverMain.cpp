/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverMain.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/08 14:36:52 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/11 16:50:18 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server/ServerHandler.hpp"

#include <iostream>

int main()
{
	std::vector<int> ports;
	ports.push_back(8080);
	ports.push_back(8081);
	ports.push_back(8082);
	ports.push_back(8083);

	ServerHandler sh(ports);
	sh.run();
	return 0;
}