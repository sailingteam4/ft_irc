/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 21:17:58 by mateo             #+#    #+#             */
/*   Updated: 2025/05/20 18:05:20 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Table.hpp"
#include "Player.hpp"
#include "Screen.hpp"

void	play(Screen &screen, Table &table, Player &player)
{
	handValue(table, table.getPlayHand(), getPokerHand(table.getPlayHand()));
	table.playHandClear();
	table.calculateUserScore();
	table.setMult(0);
	table.setTokens(0);
	table.removeHandRemains();
	table.addCardtoHand(player.getHandSize() - table.getHand().size());
	screen.clear();
	screen.putTable(table, player);
}
