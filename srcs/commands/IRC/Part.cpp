#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"
#include <sstream>

void Server::handlePart(int client_fd, const std::string& message)
{
    if (client_info.find(client_fd) == client_info.end() || 
        !client_info[client_fd].hasNick || !client_info[client_fd].hasUser)
    {
        std::string errorMsg = ":" SERVER_NAME " 451 :You have not registered\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    size_t pos = message.find("PART") + 5;
    size_t commandEnd = message.find(" :", pos);
    std::string channels_str;
    std::string part_message = "Leaving";
    
    if (commandEnd != std::string::npos) {
        channels_str = message.substr(pos, commandEnd - pos);
        part_message = message.substr(commandEnd + 2);
        
        if (part_message.find("\r") != std::string::npos)
            part_message.erase(part_message.find("\r"));
        else if (part_message.find("\n") != std::string::npos)
            part_message.erase(part_message.find("\n"));
    }
    else {
        channels_str = message.substr(pos);
        
        if (channels_str.find("\r") != std::string::npos)
            channels_str.erase(channels_str.find("\r"));
        else if (channels_str.find("\n") != std::string::npos)
            channels_str.erase(channels_str.find("\n"));
    }

    while (!channels_str.empty() && isspace(channels_str[0]))
        channels_str.erase(0, 1);
        
    if (channels_str.empty()) {
        std::string errorMsg = ":" SERVER_NAME " 461 " + client_nicknames[client_fd] + " PART :Not enough parameters\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
    std::string nickname = client_nicknames[client_fd];
    std::vector<std::string> channel_names;
    std::istringstream channels_iss(channels_str);
    std::string single_channel;
    
    while (std::getline(channels_iss, single_channel, ',')) {
        if (single_channel.empty())
            continue;
            
        if (single_channel[0] != '#')
            single_channel = "#" + single_channel;
            
        channel_names.push_back(single_channel);
    }
    
    for (size_t i = 0; i < channel_names.size(); i++) {
        std::string channel_name = channel_names[i];
        Channel* channel = findChannel(channel_name);
        
        if (channel == NULL) {
            std::string errorMsg = ":" SERVER_NAME " 403 " + nickname + " " + channel_name + " :No such channel\r\n";
            send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
            continue;
        }
        
        if (!channel->hasUser(client_fd)) {
            std::string errorMsg = ":" SERVER_NAME " 442 " + nickname + " " + channel_name + " :You're not on that channel\r\n";
            send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
            continue;
        }
        
        std::string partMessage = ":" + nickname + "!" SERVER_NAME " PART " + channel_name + " :" + part_message + "\r\n";
        send(client_fd, partMessage.c_str(), partMessage.size(), 0);
        
        broadcastToChannel(partMessage, channel_name, client_fd);
        
        channel->removeUser(client_fd);
        
        if (channel->getUsers().empty()) {
            for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
                if (it->getName() == channel_name) {
                    std::cout << "Removing empty channel: " << channel_name << std::endl;
                    channels.erase(it);
                    break;
                }
            }
        }
        
        std::cout << "Client " << nickname << " left channel: " << channel_name << " with message: " << part_message << std::endl;
    }
}