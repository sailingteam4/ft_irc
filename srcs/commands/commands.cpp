#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"
#include <sstream>

void Server::handlePass(int client_fd, const std::string& message)
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
}

void Server::handleNick(int client_fd, const std::string& message)
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
}

void Server::handlePing(int client_fd, const std::string& message)
{
    std::string pingParam = message.substr(5);
    std::string response = "PONG :" + pingParam + "\r\n";
    send(client_fd, response.c_str(), response.size(), 0);
}

void Server::handleQuit(int client_fd, const std::string& message)
{
    (void)message;
    std::string response = "Bye bye !\r\n";
    send(client_fd, response.c_str(), response.size(), 0);
    cleanupSocket(client_fd);
}

void Server::handleJoin(int client_fd, const std::string& message)
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
}

void Server::handleTopic(int client_fd, const std::string& message)
{
    size_t pos = message.find("TOPIC") + 6;
    std::string channel = message.substr(pos);
    if (channel.find("\r") != std::string::npos)
        channel.erase(channel.find("\r"));
    std::string nickname = client_nicknames[client_fd];

    std::string topicMessage = ":" + nickname + "!user@localhost TOPIC " + channel + " :New topic set\r\n";
    send(client_fd, topicMessage.c_str(), topicMessage.size(), 0);

    std::cout << "Client " << nickname << " set topic for channel: " << channel << std::endl;
}