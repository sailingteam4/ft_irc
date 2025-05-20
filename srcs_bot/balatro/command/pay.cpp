/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pay.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 06:02:53 by mateo             #+#    #+#             */
/*   Updated: 2025/05/20 18:04:03 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Card.hpp"
#include "Table.hpp"
#include "Screen.hpp"
#include "bot.hpp"
#include "SoldCard.hpp"
#include <limits>

void	Table::soldShopCard(uint32_t ind) {
	delete _shop[ind];
	_shop[ind] = new SoldCard();
}

static bool	parse(uint32_t money, std::vector<Card *> shop, std::string arg)
{
	if (!isNumber(arg))
		return false;
	long val = std::strtol(arg.c_str(), NULL, 10);
	if (val < std::numeric_limits<int>::min() || val > std::numeric_limits<int>::max())
		return false;
	if (val >= (int)shop.size())
		return false;
	if (money < shop[val]->getPrice())
		return (false);
	return true;

}

bool	pay(Screen &screen, Table &table, Player &player, std::string response)
{
	std::string	arg = response.substr(response.find("!pay ") + 5);
	if (!parse(player.getMoney(), table.getShop(), arg))
	{
		return false;
	}
	int	ind = std::atoi(arg.c_str());
	if (table.getShop()[ind]->getType() == SOLD_CARD)
		return false;
	player.addConsomCard(table.getShop()[ind]->clone());
	player.addMoney(-table.getShop()[ind]->getPrice());
	table.soldShopCard(ind);	
	screen.clear();
	screen.putShop(table, player);
	
	return true;
}
