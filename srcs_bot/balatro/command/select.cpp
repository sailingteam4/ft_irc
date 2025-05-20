/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 10:03:36 by mateo             #+#    #+#             */
/*   Updated: 2025/05/20 18:03:47 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Table.hpp"
#include "Screen.hpp"
#include "bot.hpp"
#include <sstream>
#include <set>
#include <limits>
#include <algorithm>

static bool parseArgs(const std::string& line, std::vector<int>& result, int cardLen) {
	std::istringstream iss(line);
	std::string token;
	std::set<int> seen;

	while (iss >> token) {
		if (!isNumber(token))
			return false;

		long val = std::strtol(token.c_str(), NULL, 10);
		if (val < std::numeric_limits<int>::min() || val > std::numeric_limits<int>::max())
			return false;

		int iv = static_cast<int>(val);
		if (seen.count(iv))
			return false;
		if (val >= cardLen)
			return false;

		seen.insert(iv);
		result.push_back(iv);
	}
	if (result.size() > 5)
		return (false);
	return true;
}

std::vector<PlayingCard>	removeCardWithIndexLst(std::vector<PlayingCard> hand, std::vector<int> indLst)
{
	std::sort(indLst.begin(), indLst.end(), std::greater<int>());
	
	for (int i = 0; i < (int)indLst.size(); i ++) {
		int idx = indLst[i];
		if (idx >= 0 && idx < (int)hand.size())
			hand.erase(hand.begin() + idx);
	}
	return (hand);
}

bool	Table::selectHand(std::string response, Table table)
{
	std::string	arg = response.substr(response.find("!select ") + 8);
	std::vector<int>	iCardLst;

	if (!parseArgs(arg, iCardLst, (int)table.getHand().size())) {
		return true;
	}
	for (int i = 0; i < (int)iCardLst.size(); i ++)
		_playHand.push_back(_hand[iCardLst[i]]);
	_hand = removeCardWithIndexLst(_hand, iCardLst);
	return (false);
}

bool	selectc(Screen &screen, Table &table, Player &player, std::string response)
{
	if (table.selectHand(response, table))
			return false;
	pokerHand	ph = getPokerHand(table.getPlayHand());
	screen.clear();
	table.setTokens(player.getPokerHandScore()[ph][0]);
	table.setMult(player.getPokerHandScore()[ph][1]);
	screen.putTable(table, player);
	screen.putCardList(table.getPlayHand(), 35, 10, false);
	screen.putText(pokerHandStr[ph], 1, 12);
	return true;
}
