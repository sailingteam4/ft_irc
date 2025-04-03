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


    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    std::cout << "Waiting for a connection..." << std::endl;
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    if (clientSocket == -1)
    {
        std::cerr << "Error: Could not accept connection" << std::endl;
        return 1;
    }
    
    close(listening);

    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0);
    if (result)
    {
        std::cout << host << " connected on " << service << std::endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
    }

    char buf[4096];
    while (true)
    {
        memset(buf, 0, 4096);
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == -1)
        {
            std::cerr << "Error: Could not receive data" << std::endl;
            break;
        }
        else if (bytesReceived == 0)
        {
            std::cout << "Client disconnected" << std::endl;
            break;
        }
        std::cout << "Received: " << std::string(buf, 0, bytesReceived) << std::endl;
        

        send(clientSocket, buf, bytesReceived + 1, 0);
    }

    close(clientSocket);
    std::cout << "Server shutting down..." << std::endl;

    return 0;
}