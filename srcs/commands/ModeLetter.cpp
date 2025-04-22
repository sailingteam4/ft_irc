#include "Server.hpp"
#include "Channel.hpp"
#include "error.hpp"
#include "ft_irc.hpp"

void Server::ModeOperator(char modeLetter, char sign, std::string target_value, std::string channelName, int client_fd)
{
	// if (modeLetter != 'o')
	// 	return;
	std::cout << "fduyfidsdsihfhsafdfdfdhsahllfhdshafldhshlfadlsfdsafdsafsadasdfhjfdsa" << std::endl;
	(void)modeLetter;
	Channel* channel = findChannel(channelName);
	
	if (channel->isOperator(client_fd))
	{

		if (sign == '+')
		{
			int target_client_fd = getClientSocket(target_value);
			if (target_client_fd == -1)
				{
					std::string errorMsg = ":" SERVER_NAME " 441 " + client_nicknames[client_fd] + " " + channelName + " :They aren't on that channel\r\n";
					send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
				}
			channel->setOperator(target_client_fd, 1);
		}
		else
		{
			int target_client_fd = getClientSocket(target_value);
			if (target_client_fd == -1)
			{
				std::string errorMsg = ":" SERVER_NAME " 441 " + client_nicknames[client_fd] + " " + channelName + " :They aren't on that channel\r\n";
				send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
			}
			channel->setOperator(target_client_fd, 0);
		}

	}
	else 
	{
		std::string errorMsg = ":" SERVER_NAME " 482 " + client_nicknames[client_fd] + " " + channelName + " :You're not channel operator\r\n";
		send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
	}
}
