#include "Server.hpp"

void handleClientMessage(int client_fd, const std::string& message, std::map<int, std::string>& client_nicknames, fd_set& master)
{
    std::cout << "Received from socket " << client_fd << ": " << message << std::endl;

    if (message.find("PING") != std::string::npos)
    {
        std::string response = "PONG :" + message.substr(5);
        send(client_fd, response.c_str(), response.size(), 0);
    }
    else if (message.find("QUIT") != std::string::npos)
    {
        std::string response = "Bye bye !\n";
        send(client_fd, response.c_str(), response.size(), 0);
        close(client_fd);
        FD_CLR(client_fd, &master);
        std::cout << "Client socket " << client_fd << " disconnected" << std::endl;
    }
    else if (message.find("NICK") != std::string::npos)
    {
        std::string nickname = message.substr(5);
        nickname.erase(nickname.find("\r"));
        client_nicknames[client_fd] = nickname;

        std::string response = "NICK command received. Your nickname is now " + nickname + "\n";
        send(client_fd, response.c_str(), response.size(), 0);

        std::cout << "Client socket " << client_fd << " set nickname to: " << nickname << std::endl;
    }
    else if (message.find("JOIN") != std::string::npos)
    {
        size_t pos = message.find("JOIN") + 5;
        std::string channel = message.substr(pos);
        channel.erase(channel.find("\r"));

        std::string nickname = client_nicknames[client_fd];

        std::string joinMessage = ":" + nickname + "!user@localhost JOIN :" + channel + "\r\n";
        send(client_fd, joinMessage.c_str(), joinMessage.size(), 0);

        std::string response = ":irc.example.com 331 " + nickname + " " + channel + " :No topic is set\r\n";
        send(client_fd, response.c_str(), response.size(), 0);

        response = ":irc.example.com 353 " + nickname + " = " + channel + " :@" + nickname + "\r\n";
        send(client_fd, response.c_str(), response.size(), 0);

        response = ":irc.example.com 366 " + nickname + " " + channel + " :End of /NAMES list.\r\n";
        send(client_fd, response.c_str(), response.size(), 0);

        std::cout << "Client " << nickname << " joined channel: " << channel << std::endl;
    }
    else if (message.find("TOPIC") != std::string::npos)
    {
        size_t pos = message.find("TOPIC") + 6;
        std::string channel = message.substr(pos);
        channel.erase(channel.find("\r"));
        std::string nickname = client_nicknames[client_fd];

        std::string topicMessage = ":" + nickname + "!user@localhost TOPIC " + channel + " :New topic set\r\n";
        send(client_fd, topicMessage.c_str(), topicMessage.size(), 0);

        std::cout << "Client " << nickname << " set topic for channel: " << channel << std::endl;
    }
    else
    {
        std::string response = "421 Unknown command\r\n";
        send(client_fd, response.c_str(), response.size(), 0);
    }
}
