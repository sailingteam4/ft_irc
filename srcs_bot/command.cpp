/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 19:27:17 by mbico             #+#    #+#             */
/*   Updated: 2025/05/12 12:39:55 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.hpp"
#include "Table.hpp"
#include "Card.hpp"
#include "Screen.hpp"


void	test(int sockfd)
{
	Player	player;
	Table	table(player);
	Screen	screen;
	table.firtHand(player);
	screen.putTable(table, player);
	screen.displayScreen(sockfd);
}

int	commandHandle(int32_t sockfd)
{
	std::string response = getResponse(sockfd);
    if (response.find(TAG "PING") != std::string::npos) {
		std::string pong = "PRIVMSG " CHANNEL " :PONG\r\n";
		send(sockfd, pong.c_str(), pong.length(), 0);
	}
	if (response.find("successful") != std::string::npos) { // 001 = Welcome
		std::string joinCmd = "JOIN " CHANNEL "\r\n";
		send(sockfd, joinCmd.c_str(), joinCmd.length(), 0);
	}
	if (response.find(TAG "test") != std::string::npos)
	{
		test(sockfd);
	}
	if (response.find(TAG "balatro") != std::string::npos)
	{
		gameHandle(sockfd);	
	}
	return (0);
}
