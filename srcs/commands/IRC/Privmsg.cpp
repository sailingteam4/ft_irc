#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"
#include <sstream>

void Server::handlePrivmsg(int client_fd, const std::string& message)
{
    if (client_info.find(client_fd) == client_info.end() || 
        !client_info[client_fd].hasNick || !client_info[client_fd].hasUser)
    {
        std::string errorMsg = ":" SERVER_NAME " 451 :You have not registered\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
    size_t pos = message.find("PRIVMSG") + 8;
    size_t colonPos = message.find(" :", pos);
    
    if (colonPos == std::string::npos)
    {
        std::string response = ":" SERVER_NAME " 412 :No text to send\r\n";
        send(client_fd, response.c_str(), response.size(), 0);
        return;
    }
    
    std::string target = message.substr(pos, colonPos - pos);
    std::string content = message.substr(colonPos + 2);
    
    if (content.find("\r") != std::string::npos)
        content.erase(content.find("\r"));
    else if (content.find("\n") != std::string::npos)
        content.erase(content.find("\n"));
        
    std::string nickname = client_nicknames[client_fd];
    if (nickname.empty())
    {
        std::stringstream ss;
        ss << "user" << client_fd;
        nickname = ss.str();
    }
    
    if (!target.empty() && target[0] == '#')
    {
        Channel* channel = findChannel(target);
        if (channel == NULL)
        {
            std::string errorMsg = ":" SERVER_NAME " 403 " + nickname + " " + target + " :No such channel\r\n";
            send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
            return;
        }
        
        if (!channel->hasUser(client_fd))
        {
            std::string errorMsg = ":" SERVER_NAME " 404 " + nickname + " " + target + " :Cannot send to channel\r\n";
            send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
            return;
        }
        
        std::string formattedMsg = ":" + nickname + "!" SERVER_NAME " PRIVMSG " + target + " :" + content + "\r\n";
        broadcastToChannel(formattedMsg, target, client_fd);
        
        std::cout << "Client " << nickname << " sent message to channel " << target << ": " << content << std::endl;
    }
    else
    {
        int target_fd = -1;
        for (std::map<int, std::string>::const_iterator it = client_nicknames.begin(); it != client_nicknames.end(); ++it)
        {
            if (it->second == target)
            {
                target_fd = it->first;
                break;
            }
        }
        
        if (target_fd == -1)
        {
            std::string errorMsg = ":" SERVER_NAME " 401 " + nickname + " " + target + " :No such nick/channel\r\n";
            send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
            return;
        }
        
        std::string formattedMsg = ":" + nickname + "!" SERVER_NAME " PRIVMSG " + target + " :" + content + "\r\n";
        send(target_fd, formattedMsg.c_str(), formattedMsg.size(), 0);
        
        std::cout << "Client " << nickname << " sent private message to " << target << ": " << content << std::endl;
    }
}