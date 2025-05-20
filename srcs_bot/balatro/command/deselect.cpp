/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deselect.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 18:07:16 by mateo             #+#    #+#             */
/*   Updated: 2025/05/20 18:01:17 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Table.hpp"
#include "Screen.hpp"

void	Table::insertPlayCardinHand()
{
	for (int i = 0; i < (int)_playHand.size(); i ++)
	{
		_hand.push_back(_playHand[i]);	
	}
	_hand = sortValue(_hand);
	(*this).playHandClear();
}

void	deselect(Screen &screen, Table &table, Player &player)
{
	table.insertPlayCardinHand();
	screen.clear();
	table.setTokens(0);
	table.setMult(0);
	screen.putTable(table, player);
}
