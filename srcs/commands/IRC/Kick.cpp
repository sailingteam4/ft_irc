#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"
#include <sstream>

void Server::handleKick(int client_fd, const std::string& message)
{
    if (client_info.find(client_fd) == client_info.end() || 
        !client_info[client_fd].hasNick || !client_info[client_fd].hasUser)
    {
        std::string errorMsg = ":" SERVER_NAME " 451 :You have not registered\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
    size_t pos = message.find("KICK") + 5;
    std::string params = message.substr(pos);
    
    if (params.find("\r") != std::string::npos)
        params.erase(params.find("\r"));
    else if (params.find("\n") != std::string::npos)
        params.erase(params.find("\n"));
    
    std::istringstream iss(params);
    std::string channel_name, target_nick, reason;
    
    iss >> channel_name >> target_nick;
    
    if (channel_name.empty() || target_nick.empty())
    {
        std::string nickname = client_nicknames[client_fd];
        if (nickname.empty())
        {
            std::stringstream ss;
            ss << "user" << client_fd;
            nickname = ss.str();
        }
        std::string errorMsg = ":" SERVER_NAME " 461 " + nickname + " KICK :Not enough parameters\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
    size_t colonPos = params.find(" :", iss.tellg());
    if (colonPos != std::string::npos)
        reason = params.substr(colonPos + 2);
    else
        reason = "No reason given";
    
    std::string nickname = client_nicknames[client_fd];
    if (nickname.empty())
    {
        std::stringstream ss;
        ss << "user" << client_fd;
        nickname = ss.str();
    }
    
    if (!channel_name.empty() && channel_name[0] != '#')
        channel_name = "#" + channel_name;
    
    Channel* channel = findChannel(channel_name);
    if (channel == NULL)
    {
        std::string errorMsg = ":" SERVER_NAME " 403 " + nickname + " " + channel_name + " :No such channel\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
    if (!channel->hasUser(client_fd))
    {
        std::string errorMsg = ":" SERVER_NAME " 442 " + nickname + " " + channel_name + " :You're not on that channel\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
    if (!channel->isOperator(client_fd))
    {
        std::string errorMsg = ":" SERVER_NAME " 482 " + nickname + " " + channel_name + " :You're not channel operator\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
    int target_client_fd = getClientSocket(target_nick);
    if (target_client_fd == -1)
    {
        std::string errorMsg = ":" SERVER_NAME " 401 " + nickname + " " + target_nick + " :No such nick/channel\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
    if (!channel->hasUser(target_client_fd))
    {
        std::string errorMsg = ":" SERVER_NAME " 441 " + nickname + " " + target_nick + " " + channel_name + " :They aren't on that channel\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
    std::string kickMsg = ":" + nickname + "!" SERVER_NAME " KICK " + channel_name + " " + target_nick + " :" + reason + "\r\n";
    send(client_fd, kickMsg.c_str(), kickMsg.size(), 0);
    send(target_client_fd, kickMsg.c_str(), kickMsg.size(), 0);
    broadcastToChannel(kickMsg, channel_name, client_fd);
    
    channel->removeUser(target_client_fd);
    
    std::cout << "Client " << nickname << " kicked " << target_nick << " from channel " << channel_name 
              << " with reason: " << reason << std::endl;
}