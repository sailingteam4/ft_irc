/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   use.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 06:02:53 by mateo             #+#    #+#             */
/*   Updated: 2025/05/20 18:02:39 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Card.hpp"
#include "Table.hpp"
#include "Screen.hpp"
#include "bot.hpp"
#include "SoldCard.hpp"
#include <limits>

static bool	parse(std::vector<Card *> consom, std::string arg)
{
	if (!isNumber(arg))
		return false;
	long val = std::strtol(arg.c_str(), NULL, 10);
	if (val < std::numeric_limits<int>::min() || val > std::numeric_limits<int>::max())
		return false;
	if (val >= (long)consom.size())
		return false;
	return true;

}

bool	use(Table &table, Player &player, std::string response)
{
	std::string	arg = response.substr(response.find("!use ") + 5);
	if (!parse(player.getConsomCard(), arg))
	{
		return false;
	}
	int	ind = std::atoi(arg.c_str());

	if (player.getConsomCard()[ind]->getType() == PLANET_CARD)
	{
		PlanetCard *card = (PlanetCard *)player.getConsomCard()[ind]->clone();
		player.addPokerHandLevel(card->getPokerHand(), card->getToken(), card->getMult());
		std::cerr << "token = " << card->getToken() << " mult = " << card->getMult() << std::endl;
		table.addRemainPlanet(*card);
		delete card;
	}
	delete player.getConsomCard()[ind];
	player.eraseConsomCard(ind);
	return true;
}
