/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juligonz <juligonz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/30 14:07:41 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/09 12:09:20 by juligonz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
// #include "Scanner.hpp"
// #include "ScannerHttpRequest.hpp"
#include "ParserHttpRequest.hpp"
#include "HttpRequest.hpp"

int main(int ac, char** av)
{
	(void)ac;
	(void)av;

	std::ifstream req ("./http_requests/ex2");

	// parser::http::ScannerHttpRequest scanner(req);

	// while (true)
	// {
	// 	parser::http::Token t = scanner.getToken();
	// 	std::cout << t << std::endl;
	// 	if (t.kind == parser::http::ScopedEnum::kEndOfInput)
	// 		break;
	// }
	// std::cout << std::endl;
	HttpRequest resquest = parser::http::ParserHttpRequest::create(req);

	
	// server serv;
	// long socket;

	// if (!serv.init())
	// 	return -1;
	// while (1)
	// {
	// 	std::cout << "\n--- Waiting for new connection ---\n" << std::endl;
	// 	if (!(socket = serv.accept_connection()))
	// 		return -1;
	// 	if (!serv.read_from_socket(socket))
	// 		return -1;
	// 	serv.say_hello(socket);
	// // 	serv.close_socket(socket);
	// }
	return 0;
}