#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"
#include <algorithm>

Channel* Server::findChannel(const std::string& channel_name)
{
    std::string name = channel_name;

    if (!name.empty() && name[0] != '#')
        name = "#" + name;
    
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->getName() == name)
            return &(*it);
    }
    return NULL;
}

void Server::createChannel(const std::string& channel_name)
{
    std::string name = channel_name;
    
    if (!name.empty() && name[0] != '#')
        name = "#" + name;

    if (findChannel(name) == NULL)
    {
        channels.push_back(Channel(name));
        std::cout << "Created new channel: " << name << std::endl;
    }
}

void Server::broadcastToChannel(const std::string& message, const std::string& channel_name, int exclude_fd)
{
    Channel* channel = findChannel(channel_name);
    if (channel == NULL)
        return;
    
    const std::vector<int>& users = channel->getUsers();
    for (std::vector<int>::const_iterator it = users.begin(); it != users.end(); ++it)
    {
        if (*it != exclude_fd)
        {
            send(*it, message.c_str(), message.size(), 0);
        }
    }
}

int Server::getClientSocket(std::string& nickname)
{
	std::map<int, std::string>::iterator it = client_nicknames.begin();
	while (it != client_nicknames.end())
	{
		if (it->second == nickname)
			return it->first;
		it++;
	}
	return -1;
}

std::string Server::getClientNickname(int client_fd)
{
	std::map<int, std::string>::iterator it = client_nicknames.find(client_fd);
	if (it != client_nicknames.end())
		return it->second;
	return "";
}
