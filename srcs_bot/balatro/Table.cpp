/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Table.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:33:22 by mbico             #+#    #+#             */
/*   Updated: 2025/05/07 02:17:55 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Table.hpp"
#include "Card.hpp"
#include "color.hpp"
#include "bot.hpp"
#include <iostream>

Table::Table(Player player)
{
	std::cout << YELLOW << "[System]: Table created" << RESET << std::endl;
	srand(time(0));
	_round = 1;
	_winScore = 300;
	_userScore = 0;
	_handRemains = player.getHandMax();
	_discardRemains = player.getDiscardMax();

	_mult = 0;
	_tokens = 0;

	_deck = player.getDeck();
}

Table::~Table(void)
{
	std::cout << YELLOW << "[System]: Table deleted" << RESET << std::endl;
}

void	Table::nextRound(uint32_t round)
{
	_round = round;
	_winScore = initBet[(round + 2) / 3][(round + 2) % 3];
	_userScore = 0;
	//autre truc
}

void	Table::firtHand(Player player)
{
	int	randi;

	_hand.clear();
	for (int i = 0; i < player.getHandSize(); i++)
	{
		randi = randint(0, _deck.size() - 1);
		_hand.push_back(_deck[randi]);
		_deck.erase(_deck.begin() + randi);
	}
	_hand = sortValue(_hand);
}

void	Table::displayHand(int sockfd) const
{
	std::string	str = "PRIVMSG " CHANNEL " :";
	for (int i = 0; i < (int)_hand.size(); i ++)
	{
		str += _hand[i].getStr();
		if (i != (int)_hand.size() - 1)
			str += " ";
	}
	str += "\r\n";
	std::cout << str << std::endl;
	send(sockfd, str.c_str(), str.length(), 0);
}
