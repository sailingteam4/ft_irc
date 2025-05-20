/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Screen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 21:55:13 by mbico             #+#    #+#             */
/*   Updated: 2025/05/20 17:58:57 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Screen.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Player.hpp"
#include "Table.hpp"
#include "bot.hpp"

Screen::Screen() 
{
	_lenx = 87;
	_leny = 24;
	for (uint32_t y = 0; y < _leny; y ++)
	{
		std::string	line = "";
		_screen.push_back(std::vector<std::string>());
		for (uint32_t x = 0; x < _lenx; x ++)
			_screen[y].push_back(BACKGROUND);
	}
}

Screen::~Screen(void) {
}

std::vector<std::vector<std::string> >	Screen::getScreen(void) const {
	return(_screen);
}

uint32_t	Screen::getLenX() const {
	return(_lenx);
}

uint32_t	Screen::getLenY() const {
	return (_leny);
}

void	Screen::printScreen(void) const
{
	for (uint32_t y = 0; y < _leny; y ++)
	{
		for (uint32_t x = 0; x < _lenx; x ++)
			std::cout << _screen[y][x];
		std::cout << std::endl;
	}
}

void	Screen::displayScreen(int sockfd) const
{
	for (uint32_t y = 0; y < _leny; y ++)
	{
		std::string	line = privmsg;
		for (uint32_t x = 0; x < _lenx; x ++)
			line += _screen[y][x];
		line += "\r\n";
		send(sockfd, line.c_str(), line.length(), 0);
	}
}

void	Screen::putText(std::string txt, uint32_t x, uint32_t y)
{
		if (x >= _lenx || y >= _leny || x + (int)txt.size() >= _lenx)
			return;
		for (int xr = 0; xr < (int)txt.size(); xr ++)
		{
			char c[2] = {txt[xr], '\0' };
			_screen[y][x + xr] = c;
		}
}

void	Screen::putImage(std::vector<std::vector<std::string> > img, uint32_t x, uint32_t y)
{
	if (x >= _lenx || y >= _leny || y + (int)img.size() >= _leny || x + (int)img[0].size() >= _lenx)
		return;
	for (int yr = 0; yr < (int)img.size(); yr ++)
	{
		int	xr;
		for (xr = 0; xr < (int)img[yr].size(); xr ++)
		{
			_screen[y + yr][x + xr] = img[yr][xr];
		}
	}
}

void	Screen::putCardList(std::vector<PlayingCard> hand, uint32_t x, uint32_t y, bool ind)
{
	if (x >= _lenx || y >= _leny || y + 5 >= _leny)
		return;
	(*this).putImage(hand[0].getImg(), x, y);
	if (ind)
		(*this).putText("[0]", x + 2, y + 5);
	for (int i = 0; i < (int)hand.size() - 1; i++)
	{	
		(*this).putImage(hand[i + 1].getCovertImg(), x + 7 + (4 * i), y);
		if (ind)
			(*this).putText("[" + intToString(i + 1) + "]", x + 6 + (4 * i), y + 5);
	}
}

void	Screen::clear() {	
	for (uint32_t y = 0; y < _leny; y ++)
	{
		for (uint32_t x = 0; x < _lenx; x ++)
			_screen[y][x] = BACKGROUND;
	}
}

void	Screen::putTable(Table table, Player player)
{
	
	(*this).putCardList(table.getHand(), 27, 18, true);
	(*this).putText(intToString(table.getHandRemains()), 9, 17);
	(*this).putText(intToString(table.getDiscardRemains()), 15, 17);
	(*this).putText(intToString((table.getRound() + 2) / 3), 9, 22);
	(*this).putText(intToString(table.getRound()), 15, 22);
	(*this).putText(intToString(table.getUserScore()), 12, 9);
	(*this).putText("Score at least", 2, 4);
	(*this).putText("to earn $", 3, 6);
	(*this).putText(intToString(table.getReward()), 12, 6);
	(*this).putText(intToString(table.getWinScore()), 8, 5);
	(*this).putText("$" + intToString(player.getMoney()), 12, 19);
	this->putConsom(player, 62, 1);

	(*this).putText(intToString(table.getMult()), 14, 14);
	(*this).putText(intToString(table.getTokens()), 3, 14);

}

int32_t	Screen::putWinScreen(Table table, Player player) 
{
	uint32_t	interest = player.getMoney() / 5;
	uint32_t	total;

	if (interest > 5)
		interest = 5;
	total = table.getReward() + table.getHandRemains() + interest;
	this->clear();

	(*this).putText(intToString(table.getHandRemains()), 9, 17);
	(*this).putText(intToString(table.getDiscardRemains()), 15, 17);
	(*this).putText(intToString((table.getRound() + 2) / 3), 9, 22);
	(*this).putText(intToString(table.getRound()), 15, 22);
	(*this).putText(intToString(table.getUserScore()), 12, 9);
	(*this).putText("Score at least", 2, 4);
	(*this).putText("to earn $", 3, 6);
	(*this).putText(intToString(table.getReward()), 12, 6);
	(*this).putText(intToString(table.getWinScore()), 8, 5);
	(*this).putText("$" + intToString(player.getMoney()), 12, 19);

	(*this).putText(intToString(table.getMult()), 14, 14);
	(*this).putText(intToString(table.getTokens()), 3, 14);
	this->putConsom(player, 62, 1);

	this->putText("YOU WIN", 25, 9);
	this->putText("- reward : $" + intToString(table.getReward()), 25, 10);
	this->putText("- hand remained : $" + intToString(table.getHandRemains()), 25, 11);
	this->putText("- interest : $" + intToString(interest), 25, 12);
	this->putText("----------------------------", 25, 13);
	this->putText("Total : $" + intToString(total), 25, 14);
	this->putText("type \"!next\" to go to the shop", 25, 16);
	return (total);
}

void	Screen::putGameOver(Table table, Player player)
{
	this->clear();

	(*this).putText(intToString(table.getHandRemains()), 9, 17);
	(*this).putText(intToString(table.getDiscardRemains()), 15, 17);
	(*this).putText(intToString((table.getRound() + 2) / 3), 9, 22);
	(*this).putText(intToString(table.getRound()), 15, 22);
	(*this).putText(intToString(table.getUserScore()), 12, 9);
	(*this).putText("Score at least", 2, 4);
	(*this).putText("to earn $", 3, 6);
	(*this).putText(intToString(table.getReward()), 12, 6);
	(*this).putText(intToString(table.getWinScore()), 8, 5);
	(*this).putText("$" + intToString(player.getMoney()), 12, 19);
	this->putConsom(player, 62, 1);

	(*this).putText(intToString(table.getMult()), 14, 14);
	(*this).putText(intToString(table.getTokens()), 3, 14);

	this->putText("GAME OVER", 25, 9);
	this->putText("type \"!balatro\" to retry.", 25, 10);
}

void	Screen::putShopCard(Table table, uint32_t x, uint32_t y)
{
	if (x >= _lenx || y >= _leny || y + 5 >= _leny)
		return;
	for (int i = 0; i < (int)table.getShop().size(); i++)
	{	
		(*this).putImage(table.getShop()[i]->getImg(), x + (7 * i), y);
		(*this).putText("[" + intToString(i) + "] $" + intToString(table.getShop()[i]->getPrice()), x + (7 * i), y + 5);
	}
}

void	Screen::putConsom(Player player, uint32_t x, uint32_t y)
{
	std::vector<Card *> cards = player.getConsomCard();

	if (x >= _lenx || y >= _leny || y + 5 >= _leny)
		return;
	for (int i = 0; i < (int)cards.size(); i++)
	{	
		(*this).putImage(cards[i]->getImg(), x + (7 * i), y);
		(*this).putText("[" + intToString(i) + "]", x + 2 + (7 * i), y + 5);
	}
}

void	Screen::putShop(Table table, Player player)
{
	(*this).putText(intToString(table.getHandRemains()), 9, 17);
	(*this).putText(intToString(table.getDiscardRemains()), 15, 17);
	(*this).putText(intToString((table.getRound() + 2) / 3), 9, 22);
	(*this).putText(intToString(table.getRound()), 15, 22);
	(*this).putText(intToString(table.getUserScore()), 12, 9);
	(*this).putText("$" + intToString(player.getMoney()), 12, 19);

	(*this).putText(intToString(table.getMult()), 14, 14);
	(*this).putText(intToString(table.getTokens()), 3, 14);
	this->putConsom(player, 62, 1);

	this->putShopCard(table, 42, 11);
	this->putText("to \"!roll\" pay $" + intToString(table.getRollCost()), 22, 13);
	this->putText("SHOP", 22, 9);
	this->putText("----------------------------", 22, 10);
}

void	Screen::refresh(Table table, Player player, gameStatus gs) {
	this->clear();
	if (gs == TABLE_S)
		this->putTable(table, player);
	else if (gs == SHOP)
		this->putShop(table, player);
	else if (gs == SELECT_CARD)
	{
		pokerHand	ph = getPokerHand(table.getPlayHand());
		this->putTable(table, player);
		this->putCardList(table.getPlayHand(), 35, 10, false);
		this->putText(pokerHandStr[ph], 1, 12);
	}
	else if (gs == WIN_SCREEN)
		this->putWinScreen(table, player);
		
}
