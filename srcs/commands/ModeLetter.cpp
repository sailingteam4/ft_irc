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
		std::string errorMsg = ":" SERVER_NAME " 403 " + client_nicknames[client_fd] + " " + channelName + " :No such channel\r\n";
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
				std::string errorMsg = ":" SERVER_NAME " 441 " + client_nicknames[client_fd] + " " + channelName + " :They aren't on that channel\r\n";
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
				std::string errorMsg = ":" SERVER_NAME " 441 " + client_nicknames[client_fd] + " " + channelName + " :They aren't on that channel\r\n";
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
		std::string errorMsg = ":" SERVER_NAME " 482 " + client_nicknames[client_fd] + " " + channelName + " :You're not channel operator\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
		return;
	}
}


//NE FONCTIONNE PAS
void Server::ModeTopic(char modeLetter, char sign, std::string channelName, int client_fd)
{
	if (modeLetter != 't')
		return;

	Channel* channel = findChannel(channelName);
	if (!channel)
	{
		std::string errorMsg = ":" SERVER_NAME " 403 " + client_nicknames[client_fd] + " " + channelName + " :No such channel\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
		return;
	}

	// Vérifie si le client est opérateur uniquement si le mode `t` est activé
	if (channel->isTopicProtected() && !channel->isOperator(client_fd))
	{
		std::string errorMsg = ":" SERVER_NAME " 482 " + client_nicknames[client_fd] + " " + channelName + " :You're not channel operator\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
		return;
	}

	// Active ou désactive la protection du sujet
	if (sign == '+')
	{
		channel->setTopicProtected(false);
		std::cout << "Topic protection enabled for channel: " << channelName << std::endl;
	}
	else if (sign == '-')
	{
		channel->setTopicProtected(true);
		std::cout << "Topic protection disabled for channel: " << channelName << std::endl;
	}

	// Diffuse le changement de mode
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
		std::string errorMsg = ":" SERVER_NAME " 403 " + client_nicknames[client_fd] + " " + channelName + " :No such channel\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
		return;
	}
	if (channel->isOperator(client_fd))
	{
		if (sign == '+')
		{
			if (target_value.empty())
			{
				std::string errorMsg = ":" SERVER_NAME " 461 " + client_nicknames[client_fd] + " " + channelName + " :Not enough parameters\r\n";
				send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
				return;
			}
			char* endptr = NULL;
			errno = 0;
			unsigned long valeur = strtoul(target_value.c_str(), &endptr, 10);
			if (*endptr != '\0' || errno != 0 || target_value[0] == '-')
			{

				//PAS FORCEMENT LE BON CODE D'ERREUR SOIS 472 sois 696 JE SAIS PAS TROP CA PEU AUSSI ETRE UN AUTRE

				std::string errorMsg = ":" SERVER_NAME " 696 " + client_nicknames[client_fd] + " " + channelName + " +l :Invalid limit parameter\r\n";
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
			std::string modeMsg = ":" + client_nicknames[client_fd] + " MODE " + channelName + " " + sign + "l no limit\r\n";
			send(client_fd, modeMsg.c_str(), modeMsg.size(), 0);
			broadcastToChannel(modeMsg, channelName, client_fd);
			return ;
		}
	}
	std::string errorMsg = ":" SERVER_NAME " 482 " + client_nicknames[client_fd] + " " + channelName + " :You're not channel operator\r\n";
	send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
	return;
}

void Server::ModeInvitation(char modeLetter, char sign, std::string target_value, std::string channelName, int client_fd)
{
	if (modeLetter != 'k')
		return;

	Channel* channel = findChannel(channelName);
	if (!channel)
	{
		std::string errorMsg = ":" SERVER_NAME " 403 " + client_nicknames[client_fd] + " " + channelName + " :No such channel\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
		return;
	}
	if (channel->isOperator(client_fd))
	{
		if (sign == '+')
		{
			channel->setKey(target_value);
			std::string modeMsg = ":" + client_nicknames[client_fd] + " MODE " + channelName + " +k\r\n";
			send(client_fd, modeMsg.c_str(), modeMsg.size(), 0);
			broadcastToChannel(modeMsg, channelName, client_fd);
			return;
		}
		if (sign == '-')
		{
			channel->setKey(target_value);
			std::string modeMsg = ":" + client_nicknames[client_fd] + " MODE " + channelName + sign + "k no key\r\n";
			send(client_fd, modeMsg.c_str(), modeMsg.size(), 0);
			broadcastToChannel(modeMsg, channelName, client_fd);
			return;
		}
	}
	std::string errorMsg = ":" SERVER_NAME " 482 " + client_nicknames[client_fd] + " " + channelName + " :You're not channel operator\r\n";
	send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
	return;
}