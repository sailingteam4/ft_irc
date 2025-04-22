#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"
#include <sstream>

void Server::handleNewConnection()
{
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    
    int newfd = accept(listening_socket, (sockaddr*)&client, &clientSize);
    if (newfd == -1)
    {
        std::cerr << "Error: Could not accept connection" << std::endl;
        return;
    }
    
    FD_SET(newfd, &master_set);
    if (newfd > fdmax)
    {
        fdmax = newfd;
    }
    
    client_sockets.push_back(newfd);
    client_authenticated[newfd] = false;
    
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

    std::string welcomeMessage = "Welcome to the IRC server! Please authenticate using PASS <password>\r\n";
    send(newfd, welcomeMessage.c_str(), welcomeMessage.size(), 0);
}

void Server::handleClientData(int client_fd)
{
    char buf[4096];
    memset(buf, 0, 4096);
    
    int bytesReceived = recv(client_fd, buf, 4096, 0);
    if (bytesReceived <= 0)
    {
        if (bytesReceived == 0)
        {
            std::cout << "Client socket " << client_fd << " disconnected" << std::endl;
        }
        else
        {
            std::cerr << "Error: Could not receive data from client socket " << client_fd << std::endl;
        }
        cleanupSocket(client_fd);
    }
    else
    {
        std::string message(buf, 0, bytesReceived);
        
        size_t start = 0;
        size_t end = message.find("\r\n");
        
        if (end == std::string::npos)
            end = message.find("\n");
            
        while (end != std::string::npos)
        {
            std::string cmd = message.substr(start, end - start);
            if (!cmd.empty())
            {
                handleClientMessage(client_fd, cmd);
            }
            
            start = end + (message[end] == '\r' ? 2 : 1);
            if (start >= message.size())
                break;
                
            end = message.find("\r\n", start);
            if (end == std::string::npos)
                end = message.find("\n", start);
        }
        
        if (start < message.size())
        {
            std::string cmd = message.substr(start);
            if (!cmd.empty())
            {
                handleClientMessage(client_fd, cmd);
            }
        }
    }
}

void Server::handleClientMessage(int client_fd, const std::string& message)
{
    std::cout << "Received from socket " << client_fd << ": " << message << std::endl;

    if (message.find("PASS ") == 0)
    {
        handlePass(client_fd, message);
        return;
    }

    if (!client_authenticated[client_fd] && message.find("PASS") == std::string::npos)
    {
        std::string response = "Error: You must authenticate using PASS <password> first\r\n";
        send(client_fd, response.c_str(), response.size(), 0);
        return;
    }

    if (message.find("NICK ") == 0)
    {
        handleNick(client_fd, message);
        return;
    }

    if (message.find("USER ") == 0)
    {
        handleUser(client_fd, message);
        return;
    }

    if (message.find("PING") == 0)
    {
        handlePing(client_fd, message);
        return;
    }
    
    else if (message.find("QUIT") == 0)
    {
        handleQuit(client_fd, message);
        return;
    }
    
    else if (message.find("JOIN ") == 0)
    {
        handleJoin(client_fd, message);
        return;
    }
    
    else if (message.find("TOPIC ") == 0)
    {
        handleTopic(client_fd, message);
        return;
    }

    else if (message.find("PRIVMSG ") == 0)
    {
        handlePrivmsg(client_fd, message);
        return;
    }

    else if (message.find("MODE ") == 0)
    {
        handleMode(client_fd, message);
        return;
    }

	else if (message.find("KICK ") == 0)
	{
		handleKick(client_fd, message);
		return;
	}

    else
    {
        std::cout << "Unknown command received from client " << client_fd << ": " << message << std::endl;
    }
}

bool Server::authenticateClient(int client_fd, const std::string& pass)
{
    if (pass == password)
    {
        client_authenticated[client_fd] = true;
        std::cout << "Client socket " << client_fd << " authenticated successfully" << std::endl;
        return true;
    }
    std::cout << "Client socket " << client_fd << " failed authentication" << std::endl;
    return false;
}