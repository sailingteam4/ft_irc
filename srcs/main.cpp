#include "Server.hpp"

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <fcntl.h>
#include <vector>

long stoi(const char *s)
{
    long i;
    i = 0;
    while(*s >= '0' && *s <= '9')
    {
        i = i * 10 + (*s - '0');
        s++;
    }
    return i;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Error: Invalid number of arguments" << std::endl;
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    
    std::string port(argv[1]);
    std::string password(argv[2]);

    if (password.empty())
    {
        std::cerr << "Error: Password cannot be empty" << std::endl;
        return 1;
    }

    Server server(port, password);

    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        std::cerr << "Error: Could not create socket" << std::endl;
        return 1;
    }

    int yes = 1;
    if (setsockopt(listening, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        std::cerr << "Error: Failed to set socket options" << std::endl;
        return 1;
    }

    int nb_port = stoi(argv[1]);

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(nb_port);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1)
    {
        std::cerr << "Error: Could not bind socket" << std::endl;
        return 1;
    }

    if (listen(listening, SOMAXCONN) == -1)
    {
        std::cerr << "Error: Could not listen on socket" << std::endl;
        return 1;
    }

    std::vector<int> client_sockets;
    
    fd_set master;
    fd_set read_fds;
    
    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    
    FD_SET(listening, &master);
    
    int fdmax = listening;

    std::cout << "Server is running on port " << port << std::endl;
    std::cout << "Waiting for connections..." << std::endl;

    while (true)
    {
        read_fds = master;
        
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
        {
            std::cerr << "Error: select() failed" << std::endl;
            break;
        }
        
        for (int i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds))
            {
                // Nouvelle connexion
                if (i == listening)
                {
                    sockaddr_in client;
                    socklen_t clientSize = sizeof(client);
                    
                    int newfd = accept(listening, (sockaddr*)&client, &clientSize);
                    if (newfd == -1)
                    {
                        std::cerr << "Error: Could not accept connection" << std::endl;
                    }
                    else
                    {
                        FD_SET(newfd, &master);
                        if (newfd > fdmax)
                        {
                            fdmax = newfd;
                        }
                        
                        client_sockets.push_back(newfd);
                        
                        char host[NI_MAXHOST];
                        char service[NI_MAXSERV];
                        memset(host, 0, NI_MAXHOST);
                        memset(service, 0, NI_MAXSERV);
                        
                        int result = getnameinfo((sockaddr*)&client, sizeof(client), 
                                               host, NI_MAXHOST, 
                                               service, NI_MAXSERV, 0);
                        if (result == 0)
                        {
                            std::cout << host << " connected on " << service << std::endl;
                        }
                        else
                        {
                            inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
                            std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
                        }
                        
                        std::cout << "New connection: socket fd=" << newfd 
                                 << ", ip=" << host << std::endl;
                    }
                }
				// Client qui envoie des données
                else
                {
                    char buf[4096];
                    memset(buf, 0, 4096);
                    
                    int bytesReceived = recv(i, buf, 4096, 0);
                    if (bytesReceived <= 0)
                    {
                        if (bytesReceived == 0)
                        {
                            std::cout << "Client socket " << i << " disconnected" << std::endl;
                        }
                        else
                        {
                            std::cerr << "Error: Could not receive data from client socket " << i << std::endl;
                        }
                        for (std::vector<int>::iterator it = client_sockets.begin(); it != client_sockets.end(); ++it)
                        {
                            if (*it == i)
                            {
                                client_sockets.erase(it);
                                break;
                            }
                        }
                    }
                    else
                    {
                        std::string message(buf, 0, bytesReceived);
                        std::cout << "Received from socket " << i << ": " << message << std::endl;

						std::string response = "PONG :ping";
						send(i, response.c_str(), response.size(), 0);
                    }
                }
            }
        }
    }
    
    for (std::vector<int>::iterator it = client_sockets.begin(); it != client_sockets.end(); ++it)
    {
        close(*it);
    }
    
    close(listening);
    std::cout << "Server shutting down..." << std::endl;

    return 0;
}
