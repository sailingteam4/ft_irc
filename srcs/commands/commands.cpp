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
    std::string channel_name = message.substr(pos);
    if (channel_name.find("\r") != std::string::npos)
        channel_name.erase(channel_name.find("\r"));
    else if (channel_name.find("\n") != std::string::npos)
        channel_name.erase(channel_name.find("\n"));

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
        createChannel(channel_name);
        channel = findChannel(channel_name);
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

void Server::handleTopic(int client_fd, const std::string& message)
{
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
    
    if (!channel->isOperator(client_fd)) {
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

void Server::handlePrivmsg(int client_fd, const std::string& message)
{
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
        std::string errorMsg = ":" SERVER_NAME " 401 " + nickname + " " + target + " :No such nick/channel\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
    }
}