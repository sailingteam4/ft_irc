/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   discard.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 21:22:37 by mateo             #+#    #+#             */
/*   Updated: 2025/05/20 18:04:53 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Table.hpp"
#include "Screen.hpp"

void	discard(Screen &screen, Table &table, Player &player)
{
	table.playHandClear();
	table.removeDiscardRemains();
	table.addCardtoHand(player.getHandSize() - table.getHand().size());
	screen.clear();
	screen.putTable(table, player);

}
