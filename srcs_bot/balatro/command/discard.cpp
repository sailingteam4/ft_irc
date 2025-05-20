/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   discard.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 21:22:37 by mateo             #+#    #+#             */
/*   Updated: 2025/05/18 21:24:23 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Table.hpp"
#include "Screen.hpp"

void	discard(Screen &screen, Table &table, Player &player)
{
	table.playHandClear();
	table.removeDiscardRemains();
	table.addCardtoHand(player, player.getHandSize() - table.getHand().size());
	screen.clear();
	screen.putTable(table, player);

}
