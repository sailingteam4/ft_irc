#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"
#include <cctype>

void Server::handleNick(int client_fd, const std::string& message)
{
    std::string nickname = message.substr(5);
    if (nickname.find("\r") != std::string::npos)
        nickname.erase(nickname.find("\r"));
    else if (nickname.find("\n") != std::string::npos)
        nickname.erase(nickname.find("\n"));
    
    if (nickname.find(" ") != std::string::npos)
    {
        std::string errorMsg = ":" SERVER_NAME " 432 * " + nickname + " :Error nickname: spaces are not allowed\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
    if (nickname.length() > 9)
    {
        std::string errorMsg = ":" SERVER_NAME " 432 * " + nickname + " :Error nickname: must be 9 characters or less\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
    for (size_t i = 0; i < nickname.length(); ++i)
    {
        if (isdigit(nickname[i]))
        {
            std::string errorMsg = ":" SERVER_NAME " 432 * " + nickname + " :Error nickname: numeric characters are not allowed\r\n";
            send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
            return;
        }
        
        if (!isalpha(nickname[i]))
        {
            std::string errorMsg = ":" SERVER_NAME " 432 * " + nickname + " :Error nickname: special characters are not allowed\r\n";
            send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
            return;
        }
    }
    
    for (std::map<int, std::string>::const_iterator it = client_nicknames.begin(); it != client_nicknames.end(); ++it)
    {
        if (it->first != client_fd && it->second == nickname)
        {
            std::string errorMsg = ":" SERVER_NAME " 433 * " + nickname + " :Nickname is already in use\r\n";
            send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
            return;
        }
    }
    
    std::string oldNickname = client_nicknames[client_fd];
    
    client_nicknames[client_fd] = nickname;
    
    if (client_info.find(client_fd) == client_info.end())
        client_info[client_fd] = UserInfo();
    
    client_info[client_fd].hasNick = true;

    std::string response = "NICK command received. Your nickname is now " + nickname + "\r\n";
    send(client_fd, response.c_str(), response.size(), 0);

    std::cout << "Client socket " << client_fd << " changed nickname from: " << 
        (oldNickname.empty() ? "(unset)" : oldNickname) << " to: " << nickname << std::endl;
    
    std::string nickChangeMsg = ":" + oldNickname + "!" SERVER_NAME " NICK :" + nickname + "\r\n";
    
    if (!oldNickname.empty())
    {
        for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
        {
            if (it->hasUser(client_fd))
            {
                broadcastToChannel(nickChangeMsg, it->getName(), -1);
            }
        }
    }
    
    checkRegistrationStatus(client_fd);
}