#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"
#include <sstream>

void Server::handleInvite(int client_fd, const std::string& message)
{
    if (client_info.find(client_fd) == client_info.end() || 
        !client_info[client_fd].hasNick || !client_info[client_fd].hasUser)
    {
        std::string errorMsg = ":" SERVER_NAME " 451 :You have not registered\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    size_t pos = message.find("INVITE") + 7;
    std::string params = message.substr(pos);
    if (params.find("\r") != std::string::npos)
        params.erase(params.find("\r"));
    else if (params.find("\n") != std::string::npos)
        params.erase(params.find("\n"));

    std::istringstream iss(params);
    std::string target_nick;
    std::string channel_name;
    
    iss >> target_nick;
    iss >> channel_name;

    if (target_nick.empty() || channel_name.empty())
    {
        std::string errorMsg = ":" SERVER_NAME " 461 " + client_nicknames[client_fd] + " INVITE :Not enough parameters\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    if (channel_name[0] != '#')
        channel_name = "#" + channel_name;

    int target_fd = -1;
    for (std::map<int, std::string>::const_iterator it = client_nicknames.begin(); it != client_nicknames.end(); ++it)
    {
        if (it->second == target_nick)
        {
            target_fd = it->first;
            break;
        }
    }

    if (target_fd == -1)
    {
        std::string errorMsg = ":" SERVER_NAME " 401 " + client_nicknames[client_fd] + " " + target_nick + " :No such nick/channel\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    Channel* channel = findChannel(channel_name);
    if (channel == NULL)
    {
        std::string errorMsg = ":" SERVER_NAME " 403 " + client_nicknames[client_fd] + " " + channel_name + " :No such channel\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    if (!channel->hasUser(client_fd))
    {
        std::string errorMsg = ":" SERVER_NAME " 442 " + client_nicknames[client_fd] + " " + channel_name + " :You're not on that channel\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    if (channel->isInviteOnly() && !channel->isOperator(client_fd))
    {
        std::string errorMsg = ":" SERVER_NAME " 482 " + client_nicknames[client_fd] + " " + channel_name + " :You're not channel operator\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    if (channel->hasUser(target_fd))
    {
        std::string errorMsg = ":" SERVER_NAME " 443 " + client_nicknames[client_fd] + " " + target_nick + " " + channel_name + " :is already on channel\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    channel->inviteUser(target_fd);

    std::string confirmMsg = ":" SERVER_NAME " 341 " + client_nicknames[client_fd] + " " + target_nick + " " + channel_name + "\r\n";
    send(client_fd, confirmMsg.c_str(), confirmMsg.size(), 0);

    std::string inviteMsg = ":" + client_nicknames[client_fd] + "!" SERVER_NAME " INVITE " + target_nick + " :" + channel_name + "\r\n";
    send(target_fd, inviteMsg.c_str(), inviteMsg.size(), 0);

    std::cout << "Client " << client_nicknames[client_fd] << " invited " << target_nick << " to channel: " << channel_name << std::endl;
}