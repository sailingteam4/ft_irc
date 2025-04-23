#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"
#include <sstream>

void Server::handleList(int client_fd, const std::string& message)
{
    if (client_info.find(client_fd) == client_info.end() || 
        !client_info[client_fd].hasNick || !client_info[client_fd].hasUser)
    {
        std::string errorMsg = ":" SERVER_NAME " 451 :You have not registered\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    std::string nickname = client_nicknames[client_fd];
    if (nickname.empty())
    {
        std::stringstream ss;
        ss << "user" << client_fd;
        nickname = ss.str();
    }

    size_t pos = message.find("LIST");
    if (pos == std::string::npos)
        return;
    
    std::string args = message.substr(pos + 4);
    if (args.find("\r") != std::string::npos)
        args.erase(args.find("\r"));
    else if (args.find("\n") != std::string::npos)
        args.erase(args.find("\n"));
    
    while (!args.empty() && isspace(args[0]))
        args.erase(0, 1);

    std::vector<std::string> requested_channels;
    if (!args.empty())
    {
        std::istringstream iss(args);
        std::string channel_name;
        while (std::getline(iss, channel_name, ','))
        {
            if (!channel_name.empty())
            {
                if (channel_name[0] != '#')
                    channel_name = "#" + channel_name;
                requested_channels.push_back(channel_name);
            }
        }
    }

    std::string response = ":" SERVER_NAME " 321 " + nickname + " Channel :Users Topic\r\n";
    send(client_fd, response.c_str(), response.size(), 0);

    for (std::vector<Channel>::const_iterator it = channels.begin(); it != channels.end(); ++it)
    {
        bool list_channel = requested_channels.empty();
        if (!list_channel)
        {
            for (std::vector<std::string>::const_iterator req_it = requested_channels.begin(); 
                req_it != requested_channels.end(); ++req_it)
            {
                if (*req_it == it->getName())
                {
                    list_channel = true;
                    break;
                }
            }
        }

        if (list_channel)
        {
            std::stringstream user_count;
            user_count << it->getNbUsers();
            std::string topic = it->getTopic();
            
            if (topic.empty())
                topic = "No topic set";

            response = ":" SERVER_NAME " 322 " + nickname + " " + it->getName() + " " + 
                      user_count.str() + " :" + topic + "\r\n";
            send(client_fd, response.c_str(), response.size(), 0);
        }
    }

    response = ":" SERVER_NAME " 323 " + nickname + " :End of LIST\r\n";
    send(client_fd, response.c_str(), response.size(), 0);

    std::cout << "Client " << nickname << " requested channel list" << std::endl;
}