/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientMain.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwinston <hwinston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 22:25:18 by hwinston          #+#    #+#             */
/*   Updated: 2021/08/11 10:30:19 by hwinston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "Server.hpp"
// #include "Client.hpp"

// #include <iostream>
// #include <arpa/inet.h>
// #include <string>

// int main()
// {
//     std::string hello = "Hello from client";
//     char buffer[1024] = {0};
//     sckt::Socket newSocket;

//     if (newSocket.setFd(AF_INET, SOCK_STREAM))
//     {
//         std::cout << "checkpoint" << std::endl;
//         newSocket.setAddr(AF_INET, SOCK_STREAM, 8080);
//         //sckt::setNonBlocking(newSocket.getFd());   
//         std::cout << "checkpoint" << std::endl; 
//         if (client::connect(newSocket.getFd(), newSocket.getAddr()) > 0)
//         {
//             std::cout << "checkpoint" << std::endl;
//             send(newSocket.getFd(), hello.c_str(), strlen(hello.c_str()), 0);
//             read(newSocket.getFd(), buffer, 1024);
//             std::cout << buffer << std::endl;
//         }
//     }


//     // std::vector<client::Client> clients;
//     // std::vector<client::Client>::iterator client;

//     // clients.push_back(client::Client());
    
//     // for (client = clients.begin(); client != clients.end(); client++)
//     // {
//     //     sckt::fd_type fd;
//     //     if ((fd = socket(AF_INET, SOCK_STREAM, 0)) != INVALID_FD)
//     //     {
//     //         client->setFd(fd);
//     //         sckt::setNonBlocking(client->getFd());
//     //         sckt::addr_type addr = {0};
//     //         client->setAddr(addr);
//     //         client::connect(fd, addr);
//     //     }
//     // }

//     return 0;
// }



#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    int sock = 0; long valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    //Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}