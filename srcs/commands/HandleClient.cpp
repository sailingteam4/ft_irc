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
        size_t pos = message.find("PASS") + 5;
        std::string pass = message.substr(pos);
        if (pass.find("\r") != std::string::npos)
            pass.erase(pass.find("\r"));
        else if (pass.find("\n") != std::string::npos)
            pass.erase(pass.find("\n"));
        
        if (authenticateClient(client_fd, pass))
        {
            std::string response = "Authentication successful\r\n";
            send(client_fd, response.c_str(), response.size(), 0);
        }
        else
        {
            std::string response = "Authentication failed: invalid password\r\n";
            send(client_fd, response.c_str(), response.size(), 0);
        }
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
        std::string nickname = message.substr(5);
        if (nickname.find("\r") != std::string::npos)
            nickname.erase(nickname.find("\r"));
        else if (nickname.find("\n") != std::string::npos)
            nickname.erase(nickname.find("\n"));
        
        client_nicknames[client_fd] = nickname;

        std::string response = "NICK command received. Your nickname is now " + nickname + "\r\n";
        send(client_fd, response.c_str(), response.size(), 0);

        std::cout << "Client socket " << client_fd << " set nickname to: " << nickname << std::endl;
        return;
    }

    if (message.find("PING") == 0)
    {
        std::string pingParam = message.substr(5);
        std::string response = "PONG :" + pingParam + "\r\n";
        send(client_fd, response.c_str(), response.size(), 0);
        return;
    }
    
    else if (message.find("QUIT") == 0)
    {
        std::string response = "Bye bye !\r\n";
        send(client_fd, response.c_str(), response.size(), 0);
        cleanupSocket(client_fd);
        return;
    }
    
    else if (message.find("JOIN ") == 0)
    {
        size_t pos = message.find("JOIN") + 5;
        std::string channel = message.substr(pos);
        if (channel.find("\r") != std::string::npos)
            channel.erase(channel.find("\r"));
        else if (channel.find("\n") != std::string::npos)
            channel.erase(channel.find("\n"));

        std::string nickname = client_nicknames[client_fd];
        if (nickname.empty())
        {
            std::stringstream ss;
            ss << "user" << client_fd;
            nickname = ss.str();
        }

        if (channel[0] != '#')
            channel = "#" + channel;

        std::string joinMessage = ":" + nickname + "!irc.example.com JOIN :" + channel + "\r\n";
        send(client_fd, joinMessage.c_str(), joinMessage.size(), 0);

        std::string response = ":irc.example.com 331 " + nickname + " " + channel + " :No topic is set\r\n";
        send(client_fd, response.c_str(), response.size(), 0);

        response = ":irc.example.com 353 " + nickname + " = " + channel + " :@" + nickname + "\r\n";
        send(client_fd, response.c_str(), response.size(), 0);

        response = ":irc.example.com 366 " + nickname + " " + channel + " :End of /NAMES list.\r\n";
        send(client_fd, response.c_str(), response.size(), 0);

        std::cout << "Client " << nickname << " joined channel: " << channel << std::endl;
        return;
    }
    
    else if (message.find("TOPIC ") == 0)
    {
        size_t pos = message.find("TOPIC") + 6;
        std::string channel = message.substr(pos);
        if (channel.find("\r") != std::string::npos)
            channel.erase(channel.find("\r"));
        std::string nickname = client_nicknames[client_fd];

        std::string topicMessage = ":" + nickname + "!user@localhost TOPIC " + channel + " :New topic set\r\n";
        send(client_fd, topicMessage.c_str(), topicMessage.size(), 0);

        std::cout << "Client " << nickname << " set topic for channel: " << channel << std::endl;
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