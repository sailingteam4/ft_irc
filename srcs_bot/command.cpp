/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 19:27:17 by mbico             #+#    #+#             */
/*   Updated: 2025/05/09 17:59:33 by mateo            ###   ########.fr       */
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
	screen.putImage(table.getHand()[0].getImg(), 30, 18);
	screen.putImage(table.getHand()[1].getCovertImg(), 37, 18);
	screen.putImage(table.getHand()[2].getCovertImg(), 41, 18);
	screen.putImage(table.getHand()[3].getCovertImg(), 45, 18);
	screen.putImage(table.getHand()[4].getCovertImg(), 49, 18);
	screen.putImage(table.getHand()[5].getCovertImg(), 53, 18);
	screen.putImage(table.getHand()[6].getCovertImg(), 57, 18);
	screen.putImage(table.getHand()[7].getCovertImg(), 61, 18);

	
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
