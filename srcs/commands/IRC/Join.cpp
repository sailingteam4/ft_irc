#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"
#include <sstream>

bool isValidChannelName(const std::string& name)
{
    if (name.length() < 2 || name.length() > 64)
        return false;
    
    if (name[0] != '#')
        return false;
    
    for (size_t i = 1; i < name.length(); i++)
    {
        char c = name[i];
        if (c <= 32 || c == 127 ||
            c == ',' || c == ':' || c == ';' ||
            c == '#' || c == '@' || c == '!' ||
            c == '&' || c == '+' || c == '%' ||
            c == '*' || c == '?' || c == '$' ||
            c == '(' || c == ')' || c == '<' || c == '>' ||
            c == '"' || c == '\'')
        {
            return false;
        }
    }
    return true;
}

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
    std::string params = message.substr(pos);
    if (params.find("\r") != std::string::npos)
        params.erase(params.find("\r"));
    else if (params.find("\n") != std::string::npos)
        params.erase(params.find("\n"));
    
    std::istringstream iss(params);
    std::string channels_str;
    std::string keys_str;
    
    iss >> channels_str;
    iss >> keys_str;

    std::string nickname = client_nicknames[client_fd];
    if (nickname.empty())
    {
        std::stringstream ss;
        ss << "user" << client_fd;
        nickname = ss.str();
    }

    std::vector<std::string> channel_names;
    std::istringstream channels_iss(channels_str);
    std::string single_channel;
    
    while (std::getline(channels_iss, single_channel, ','))
    {
        if (single_channel.empty())
            continue;
            
        if (single_channel[0] != '#')
        {
            std::string errorMsg = ":" SERVER_NAME " 403 " + nickname + " " + single_channel + " :Invalid channel name (missing # prefix)\r\n";
            send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
            continue;
        }
            
        if (!isValidChannelName(single_channel))
        {
            std::string errorMsg = ":" SERVER_NAME " 403 " + nickname + " " + single_channel + " :Invalid channel name\r\n";
            send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
            continue;
        }
            
        channel_names.push_back(single_channel);
    }
    
    std::vector<std::string> keys;
    std::istringstream keys_iss(keys_str);
    std::string single_key;
    
    while (std::getline(keys_iss, single_key, ','))
        keys.push_back(single_key);

    for (size_t i = 0; i < channel_names.size(); i++)
    {
        std::string channel_name = channel_names[i];
        std::string key = (i < keys.size()) ? keys[i] : "";
        
        Channel* channel = findChannel(channel_name);
        if (channel == NULL)
        {
            createChannel(channel_name);
            channel = findChannel(channel_name);
        }

        if (channel->hasKey() && (i >= keys.size() || channel->getKey() != key))
        {
            std::string errorMsg = ":" SERVER_NAME " 475 " + nickname + " " + channel_name + " :Cannot join channel (+k) - bad key\r\n";
            send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
            continue;
        }

        if (channel->isInviteOnly() && !channel->isUserInvited(client_fd))
        {
            std::string errorMsg = ":" SERVER_NAME " 473 " + nickname + " " + channel_name + " :Cannot join channel (+i) - you must be invited\r\n";
            send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
            continue;
        }

        if (channel->hasUserLimit() && channel->getNbUsers() >= channel->getUserLimit())
        {
            std::string errorMsg = ":" SERVER_NAME " 471 " + nickname + " " + channel_name + " :Cannot join channel (+l) - channel is full\r\n";
            send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
            continue;
        }

		if (channel->hasUser(client_fd))
		{
			std::string errorMsg = ":" SERVER_NAME " 443 " + nickname + " " + channel_name + " :is already on channel\r\n";
			send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
			continue;
		}

		if (channel->isInviteOnly() && channel->isUserInvited(client_fd))
			channel->uninviteUser(client_fd);

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