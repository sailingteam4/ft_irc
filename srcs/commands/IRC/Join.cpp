#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"
#include <sstream>

void Server::handleJoin(int client_fd, const std::string& message)
{
    if (client_info.find(client_fd) == client_info.end() || 
        !client_info[client_fd].hasNick || !client_info[client_fd].hasUser)
    {
        std::string errorMsg = ":" SERVER_NAME " 451 :You have not registered\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    size_t pos = message.find("JOIN") + 5;
    std::string channels_str = message.substr(pos);
    if (channels_str.find("\r") != std::string::npos)
        channels_str.erase(channels_str.find("\r"));
    else if (channels_str.find("\n") != std::string::npos)
        channels_str.erase(channels_str.find("\n"));

    std::string nickname = client_nicknames[client_fd];
    if (nickname.empty())
    {
        std::stringstream ss;
        ss << "user" << client_fd;
        nickname = ss.str();
    }

    std::vector<std::string> channel_names;
    std::istringstream iss(channels_str);
    std::string single_channel;
    
    while (std::getline(iss, single_channel, ','))
    {
        if (single_channel.empty())
            continue;
            
        if (single_channel[0] != '#')
            single_channel = "#" + single_channel;
            
        channel_names.push_back(single_channel);
    }

    for (size_t i = 0; i < channel_names.size(); i++)
    {
        std::string channel_name = channel_names[i];
        
        Channel* channel = findChannel(channel_name);
        if (channel == NULL)
        {
            createChannel(channel_name);
            channel = findChannel(channel_name);
        }

        if (channel->hasUserLimit() && channel->getNbUsers() >= channel->getUserLimit())
        {
            std::string errorMsg = ":" SERVER_NAME " 471 " + nickname + " " + channel_name + " :Cannot join channel (+l) - channel is full\r\n";
            send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
            continue;
        }

        channel->addUser(client_fd);
        
        std::string joinMessage = ":" + nickname + "!" SERVER_NAME " JOIN :" + channel_name + "\r\n";
        send(client_fd, joinMessage.c_str(), joinMessage.size(), 0);

        std::string broadcastMsg = ":" + nickname + "!" SERVER_NAME " JOIN :" + channel_name + "\r\n";
        broadcastToChannel(broadcastMsg, channel_name, client_fd);

        std::string topic = channel->getTopic();
        std::string response;
        if (topic.empty())
            response = ":" SERVER_NAME " 331 " + nickname + " " + channel_name + " :No topic is set\r\n";
        else
            response = ":" SERVER_NAME " 332 " + nickname + " " + channel_name + " :" + topic + "\r\n";
        send(client_fd, response.c_str(), response.size(), 0);

        response = ":" SERVER_NAME " 353 " + nickname + " = " + channel_name + " :" + channel->getUserList(client_nicknames) + "\r\n";
        send(client_fd, response.c_str(), response.size(), 0);
        
        response = ":" SERVER_NAME " 366 " + nickname + " " + channel_name + " :End of /NAMES list.\r\n";
        send(client_fd, response.c_str(), response.size(), 0);

        std::cout << "Client " << nickname << " joined channel: " << channel_name << std::endl;
    }
}