/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/08 14:36:52 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/08 23:42:29 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

#include <iostream>

#define PORT 8180

int main()
{
    std::cout << "---- Webserv 42 ----" << std::endl;
    
    Server server(PORT);

    while (true)
    {
        sckt::addr_type newClientAddr = {0};
        sckt::fd_type newClientFd = server.allowLink(newClientAddr, sizeof(newClientAddr));

        if (newClientFd > 0)
        {
            sckt::setNonBlocking(newClientFd);
            client::Client newClient;
            newClient.setFd(newClientFd);
            newClient.setAddr(newClientAddr);
            server.addNewClient(newClient);
        }

        std::vector<client::Client> clients = server.getClients();
        std::vector<client::Client>::iterator it;

        for(it = clients.begin(); it != clients.end(); it++)
        {
            bool disconnect = false;
            sckt::fd_type clientFd = it->getFd();
            
            
            char buffer[3000] = {0};
			int ret = recv(clientFd, buffer, 3000 - 1, 0);
            if (ret == 0)
                disconnect = true;
            else if (ret < 0)
            {
                // std::cerr << "Error: recv()" << std::endl;
                if (ret != EWOULDBLOCK)
                    disconnect = true;
            }
            else
            {
                std::cout << buffer << std::endl;

                const char* hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 7\n\nYo man!";
                ret = send(clientFd, hello, strlen(hello), 0);
                if (ret <= 0)
                    disconnect = true; 
                if (disconnect)
                    server.removeClient(it);
            }
        }
    }
    return 0;
}