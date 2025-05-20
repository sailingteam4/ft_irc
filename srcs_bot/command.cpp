/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 19:27:17 by mbico             #+#    #+#             */
/*   Updated: 2025/05/20 16:25:14 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.hpp"
#include "Table.hpp"
#include "Screen.hpp"
#include <sys/select.h>


int	commandHandle(int32_t sockfd, fd_set master)
{
	std::string response = getResponse(sockfd, master);
    if (response.find(TAG "ping") != std::string::npos) {
		std::string pong = "PRIVMSG " CHANNEL " :PONG\r\n";
		send(sockfd, pong.c_str(), pong.length(), 0);
	}
	if (response.find("successful") != std::string::npos) { // 001 = Welcome
		std::string joinCmd = "JOIN " CHANNEL "\r\n";
		send(sockfd, joinCmd.c_str(), joinCmd.length(), 0);
	}
	if (response.find(TAG "exit") != std::string::npos)
	{
		return (1);
	}
	if (response.find(TAG "balatro") != std::string::npos)
	{
		gameHandle(sockfd, master);	
	}
	return (0);
}
