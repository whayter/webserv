/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/30 14:07:41 by hwinston          #+#    #+#             */
/*   Updated: 2021/07/30 14:48:22 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"
#include "../includes/server.hpp"

int main(int ac, char** av)
{
	(void)ac;
	(void)av;
	
	server serv;
	long socket;

	if (!serv.init())
		return -1;
	while (1)
	{
		std::cout << "\n--- Waiting for new connection ---\n" << std::endl;
		if (!(socket = serv.accept_connection()))
			return -1;
		if (!serv.read_from_socket(socket))
			return -1;
		serv.say_hello(socket);
		serv.close_socket(socket);
	}
	return 0;
}