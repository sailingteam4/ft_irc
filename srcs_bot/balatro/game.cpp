/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 10:21:02 by mateo             #+#    #+#             */
/*   Updated: 2025/05/12 13:41:01 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Table.hpp"
#include "Player.hpp"
#include "Screen.hpp"
#include "bot.hpp"
#include <string>


void	gameHandle(int sockfd)
{
	gameStatus	game = TABLE_S;
	Player	player;
	Table	table(player);
	Screen	screen;

	table.firtHand(player);
	screen.putTable(table, player);
	screen.displayScreen(sockfd);
	while (game)
	{
		std::string response = getResponse(sockfd);
		if (response.find(TAG "select ") != std::string::npos && game == TABLE_S && select(screen, table, player, response))
		{
			screen.displayScreen(sockfd);
			game = SELECT_CARD;
		}
		else if (response.find(TAG "deselect") != std::string::npos && game == SELECT_CARD)
		{
			deselect(screen, table, player);
			screen.displayScreen(sockfd);
			game = TABLE_S;
		}
		else if (response.find(TAG "play") != std::string::npos && game == SELECT_CARD && table.getHandRemains())
		{
			handValue(table, table.getPlayHand(), getPokerHand(table.getPlayHand()));
			table.playHandClear();
			table.calculateUserScore(player);
			table.setMult(0);
			table.setTokens(0);
			table.removeHandRemains();
			table.addCardtoHand(player, player.getHandSize() - table.getHand().size());
			screen.clear();
			screen.putTable(table, player);
			screen.displayScreen(sockfd);
			game = TABLE_S;
		}
		else if (response.find(TAG "discard") != std::string::npos && game == SELECT_CARD && table.getDiscardRemains())
		{
			table.playHandClear();
			table.removeDiscardRemains();
			table.addCardtoHand(player, player.getHandSize() - table.getHand().size());
			screen.clear();
			screen.putTable(table, player);
			screen.displayScreen(sockfd);
			game = TABLE_S;
		}
	}
}
