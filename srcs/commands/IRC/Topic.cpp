#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"
#include <sstream>

void Server::handleTopic(int client_fd, const std::string& message)
{
    if (client_info.find(client_fd) == client_info.end() || 
        !client_info[client_fd].hasNick || !client_info[client_fd].hasUser)
    {
        std::string errorMsg = ":" SERVER_NAME " 451 :You have not registered\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
    size_t pos = message.find("TOPIC") + 6;
    std::string params = message.substr(pos);
    
    if (params.find("\r") != std::string::npos)
        params.erase(params.find("\r"));
    else if (params.find("\n") != std::string::npos)
        params.erase(params.find("\n"));
    
    std::string channel_name;
    std::string new_topic = "";
    
    size_t colonPos = params.find(" :");
    if (colonPos != std::string::npos) {
        channel_name = params.substr(0, colonPos);
        new_topic = params.substr(colonPos + 2);
    } else {
        channel_name = params;
    }
    
    std::string nickname = client_nicknames[client_fd];
    if (nickname.empty()) {
        std::stringstream ss;
        ss << "user" << client_fd;
        nickname = ss.str();
    }
    
    if (!channel_name.empty() && channel_name[0] != '#')
        channel_name = "#" + channel_name;
    
    Channel* channel = findChannel(channel_name);
    if (channel == NULL) {
        std::string errorMsg = ":" SERVER_NAME " 403 " + nickname + " " + channel_name + " :No such channel\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
    if (!channel->hasUser(client_fd)) {
        std::string errorMsg = ":" SERVER_NAME " 442 " + nickname + " " + channel_name + " :You're not on that channel\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
    if (colonPos == std::string::npos) {
        std::string topic = channel->getTopic();
        std::string response;
        
        if (topic.empty())
            response = ":" SERVER_NAME " 331 " + nickname + " " + channel_name + " :No topic is set\r\n";
        else
            response = ":" SERVER_NAME " 332 " + nickname + " " + channel_name + " :" + topic + "\r\n";
            
        send(client_fd, response.c_str(), response.size(), 0);
        return;
    }
    
    if (channel->isTopicProtected() && !channel->isOperator(client_fd)) {
        std::string errorMsg = ":" SERVER_NAME " 482 " + nickname + " " + channel_name + " :You're not channel operator\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
    channel->setTopic(new_topic);
    
    std::string topicMessage = ":" + nickname + "!" SERVER_NAME " TOPIC " + channel_name + " :" + new_topic + "\r\n";
    send(client_fd, topicMessage.c_str(), topicMessage.size(), 0);
    
    broadcastToChannel(topicMessage, channel_name, client_fd);
    
    std::cout << "Client " << nickname << " set topic for channel " << channel_name << ": " << new_topic << std::endl;
}