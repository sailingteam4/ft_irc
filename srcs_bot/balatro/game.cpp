/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 10:21:02 by mateo             #+#    #+#             */
/*   Updated: 2025/05/20 18:07:19 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Table.hpp"
#include "Player.hpp"
#include "Screen.hpp"
#include "bot.hpp"
#include "PlanetCard.hpp"
#include <string>


void	gameHandle(int sockfd, fd_set master)
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
		//command event
		std::string response = getResponse(sockfd, master);
		if (response.find(TAG "select ") != std::string::npos && game == TABLE_S && selectc(screen, table, player, response))
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
			play(screen, table, player);
			screen.displayScreen(sockfd);
			game = TABLE_S;
		}
		else if (response.find(TAG "discard") != std::string::npos && game == SELECT_CARD && table.getDiscardRemains())
		{
			discard(screen, table, player);
			screen.displayScreen(sockfd);
			game = TABLE_S;
		}
		else if (response.find(TAG "next") != std::string::npos && game == WIN_SCREEN)
		{
			screen.clear();
			table.nextRound(player);
			table.rollShop();
			screen.putShop(table, player);
			screen.displayScreen(sockfd);
			game = SHOP;
		}
		else if (response.find(TAG "next") != std::string::npos && game == SHOP)
		{
			table.firtHand(player);
			screen.clear();
			screen.putTable(table, player);
			screen.displayScreen(sockfd);
			game = TABLE_S;
		}
		else if (response.find(TAG "roll") != std::string::npos && game == SHOP && player.getMoney() >= table.getRollCost())
		{
			screen.clear();
			table.rollShop();
			player.addMoney(-table.getRollCost());
			table.addRollCost(1);
			screen.putShop(table, player);
			screen.displayScreen(sockfd);
		}
		else if (response.find(TAG "pay ") != std::string::npos && game == SHOP && pay(screen, table, player, response))
		{
			screen.displayScreen(sockfd);
		}
		else if (response.find(TAG "use ") != std::string::npos && use(table, player, response))
		{	
			screen.refresh(table, player, game);
			screen.displayScreen(sockfd);
		}
		else if (response.find(TAG "exit") != std::string::npos)
		{
			screen.putGameOver(table, player);
			screen.displayScreen(sockfd);
			table.free();
			player.free();
			game = NOGAME;

		}

		//trigger event
		if (table.getUserScore() >= table.getWinScore() && game != WIN_SCREEN && game != NOGAME)
		{
			player.addMoney(screen.putWinScreen(table, player));
			screen.displayScreen(sockfd);
			game = WIN_SCREEN;
		}
		if ((table.getHandRemains() == 0 && table.getUserScore() < table.getWinScore()) || stopServer)
		{
			screen.putGameOver(table, player);
			screen.displayScreen(sockfd);
			table.free();
			player.free();
			game = NOGAME;
		}
	}
}
