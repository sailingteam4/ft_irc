#include "Server.hpp"
#include "Channel.hpp"
#include "error.hpp"
#include "ft_irc.hpp"
#include <cerrno>
#include <cstdlib>

void Server::ModeOperator(char modeLetter, char sign, std::string target_value, std::string channelName, int client_fd)
{
	if (modeLetter != 'o')
		return;
	Channel* channel = findChannel(channelName);
	if (!channel)
	{
		std::string errorMsg = ERR_NOSUCHCHANNEL(client_nicknames[client_fd], channelName);
		send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
		return;
	}
	if (channel->isOperator(client_fd))
	{
		if (sign == '+')
		{
			int target_client_fd = getClientSocket(target_value);
			if (target_client_fd == -1)
			{
				std::string errorMsg = ERR_USERNOTINCHANNEL(client_nicknames[client_fd], target_value, channelName);
				send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
				return;
			}
			else
			{
				channel->setOperator(target_client_fd, 1);
				std::string modeMsg = ":" + client_nicknames[client_fd] + " MODE " + channelName + " +o " + target_value + "\r\n";
				send(client_fd, modeMsg.c_str(), modeMsg.size(), 0);
				broadcastToChannel(modeMsg, channelName, client_fd);
			}
		}
		else
		{
			int target_client_fd = getClientSocket(target_value);
			if (target_client_fd == -1)
			{
				std::string errorMsg = ERR_USERNOTINCHANNEL(client_nicknames[client_fd], target_value, channelName);
				send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
				return;
			}
			else
			{
				channel->setOperator(target_client_fd, 0);
				std::string modeMsg = ":" + client_nicknames[client_fd] + " MODE " + channelName + " -o " + target_value + "\r\n";
				send(client_fd, modeMsg.c_str(), modeMsg.size(), 0);
				broadcastToChannel(modeMsg, channelName, client_fd);
				
			}
		}
	}
	else 
	{
		std::string errorMsg = ERR_NOTCHANNELOP(client_nicknames[client_fd], channelName);
		send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
		return;
	}
}

void Server::ModeTopic(char modeLetter, char sign, std::string channelName, int client_fd)
{
	if (modeLetter != 't')
		return;

	Channel* channel = findChannel(channelName);
	if (!channel)
	{
		std::string errorMsg = ERR_NOSUCHCHANNEL(client_nicknames[client_fd], channelName);
		send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
		return;
	}

	if (channel->isTopicProtected() && !channel->isOperator(client_fd))
	{
		std::string errorMsg = ERR_NOTCHANNELOP(client_nicknames[client_fd], channelName);
		send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
		return;
	}

	if (sign == '+')
	{
		channel->setTopicProtected(true);
		std::cout << "Topic protection enabled for channel: " << channelName << std::endl;
	}
	else if (sign == '-')
	{
		channel->setTopicProtected(false);
		std::cout << "Topic protection disabled for channel: " << channelName << std::endl;
	}

	std::string modeMsg = ":" + client_nicknames[client_fd] + " MODE " + channelName + " " + sign + "t\r\n";
	send(client_fd, modeMsg.c_str(), modeMsg.size(), 0);
	broadcastToChannel(modeMsg, channelName, client_fd);
}

void Server::ModeLimit(char modeLetter, char sign, std::string target_value, std::string channelName, int client_fd)
{
	if (modeLetter != 'l')
		return;

	Channel* channel = findChannel(channelName);
	if (!channel)
	{
		std::string errorMsg = ERR_NOSUCHCHANNEL(client_nicknames[client_fd], channelName);
		send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
		return;
	}
	if (channel->isOperator(client_fd))
	{
		if (sign == '+')
		{
			if (target_value.empty())
			{
				std::string errorMsg = ERR_NEEDMOREPARAMS(client_nicknames[client_fd], "MODE");
				send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
				return;
			}
			char* endptr = NULL;
			errno = 0;
			unsigned long valeur = strtoul(target_value.c_str(), &endptr, 10);
			if (*endptr != '\0' || errno != 0 || target_value[0] == '-')
			{
				std::string errorMsg = ERR_UNKNOWNMODE(client_nicknames[client_fd], channelName, "+l");
				send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
				return;
			}
	
			unsigned long limit = valeur;
			if (limit > 100000)
				limit = 100000;
			channel->setUserLimit(limit);
			std::string modeMsg = ":" + client_nicknames[client_fd] + " MODE " + channelName + " " + sign + "l " + target_value + "\r\n";
			send(client_fd, modeMsg.c_str(), modeMsg.size(), 0);
			broadcastToChannel(modeMsg, channelName, client_fd);
			return ;
		}
		if (sign == '-')
		{
			channel->setUserLimit(0);
			std::string modeMsg = ":" + client_nicknames[client_fd] + " MODE " + channelName + " " + sign + "l\r\n";
			send(client_fd, modeMsg.c_str(), modeMsg.size(), 0);
			broadcastToChannel(modeMsg, channelName, client_fd);
			return ;
		}
	}
	std::string errorMsg = ERR_NOTCHANNELOP(client_nicknames[client_fd], channelName);
	send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
	return;
}

void Server::ModeKey(char modeLetter, char sign, std::string target_value, std::string channelName, int client_fd)
{
	if (modeLetter != 'k')
		return;

	Channel* channel = findChannel(channelName);
	if (!channel)
	{
		std::string errorMsg = ERR_NOSUCHCHANNEL(client_nicknames[client_fd], channelName);
		send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
		return;
	}
	if (channel->isOperator(client_fd))
	{
		if (sign == '+')
		{
			if (target_value.empty())
			{
				std::string errorMsg = ERR_NEEDMOREPARAMS(client_nicknames[client_fd], "MODE");
				send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
				return;
			}
			channel->setKey(target_value);
			std::string modeMsg = ":" + client_nicknames[client_fd] + " MODE " + channelName + " +k " + target_value + "\r\n";
			send(client_fd, modeMsg.c_str(), modeMsg.size(), 0);
			broadcastToChannel(modeMsg, channelName, client_fd);
			return;
		}
		if (sign == '-')
		{
			channel->setKey("");
			std::string modeMsg = ":" + client_nicknames[client_fd] + " MODE " + channelName + " -k\r\n";
			send(client_fd, modeMsg.c_str(), modeMsg.size(), 0);
			broadcastToChannel(modeMsg, channelName, client_fd);
			return;
		}
	}
	std::string errorMsg = ERR_NOTCHANNELOP(client_nicknames[client_fd], channelName);
	send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
	return;
}

void Server::ModeInvitation(char modeLetter, char sign, std::string channelName, int client_fd)
{
	if (modeLetter != 'i')
	return;

	Channel* channel = findChannel(channelName);
	if (!channel)
	{
		std::string errorMsg = ERR_NOSUCHCHANNEL(client_nicknames[client_fd], channelName);
		send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
		return;
	}
	if (channel->isOperator(client_fd))
	{
			if (sign == '+')
		{
			channel->setInviteOnly(true);
			std::string modeMsg = ":" + client_nicknames[client_fd] + " MODE " + channelName + " +i\r\n";
			send(client_fd, modeMsg.c_str(), modeMsg.size(), 0);
			broadcastToChannel(modeMsg, channelName, client_fd);
			return;
		}
		if (sign == '-')
		{
			channel->setInviteOnly(false);
			std::string modeMsg = ":" + client_nicknames[client_fd] + " MODE " + channelName + " -i\r\n";
			send(client_fd, modeMsg.c_str(), modeMsg.size(), 0);
			broadcastToChannel(modeMsg, channelName, client_fd);
			return;
		}
	}
	std::string errorMsg = ERR_NOTCHANNELOP(client_nicknames[client_fd], channelName);
	send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
	return;
}