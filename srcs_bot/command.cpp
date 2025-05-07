/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 19:27:17 by mbico             #+#    #+#             */
/*   Updated: 2025/05/07 23:43:49 by mbico            ###   ########.fr       */
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
	screen.putImage(table.getHand()[0].getImg(), 3, 5);
	/* screen.putImage(table.getHand()[1].getImg(), 50, 5); */


	screen.displayScreen(sockfd);
}

int	commandHandle(int32_t sockfd)
{
	char buffer[512];
	int bytes_received;

	memset(buffer, 0, sizeof(buffer));
	bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0)
		return (1);
    std::string response(buffer);
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
	return (0);
}
