/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Table.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:33:22 by mbico             #+#    #+#             */
/*   Updated: 2025/05/06 03:11:05 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Table.hpp"
#include "Card.hpp"
#include "color.hpp"
#include <iostream>

Table::Table(Player player)
{
	std::cout << YELLOW << "[System]: Table created" << RESET << std::endl;
	_round = 1;
	_winScore = 300;
	_userScore = 0;
	_cardRemains = player.getDeckSize();
	_handRemains = player.getHandMax();
	_discardRemains = player.getDiscardMax();

	_mult = 0;
	_tokens = 0;
	
	_playHand = cardList(5);
	_Hand = cardList(player.getHandSize());
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
