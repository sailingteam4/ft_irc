/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 10:03:36 by mateo             #+#    #+#             */
/*   Updated: 2025/05/12 12:33:30 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Table.hpp"
#include "Screen.hpp"
#include "bot.hpp"
#include <sstream>
#include <set>
#include <limits>
#include <algorithm>

bool isNumber(const std::string& s) {
	if (s.empty()) return false;

	std::size_t i = 0;
	if (s[0] == '-' || s[0] == '+') i++;
	for (; i < s.size(); ++i)
		if (!std::isdigit(s[i]))
			return false;
	return true;
}

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
			return false; // doublon
		if (val >= cardLen)
			return false;

		seen.insert(iv);
		result.push_back(iv);
	}
	return true;
}

std::vector<Card>	removeCardWithIndexLst(std::vector<Card> hand, std::vector<int> indLst)
{
	std::sort(indLst.begin(), indLst.end(), std::greater<int>());
	
	for (int i = 0; i < indLst.size(); i ++) {
		int idx = indLst[i];
		if (idx >= 0 && idx < hand.size())
			hand.erase(hand.begin() + idx);
	}
	return (hand);
}

bool	Table::selectHand(std::string response, Table table)
{
	std::string	arg = response.substr(response.find("!select ") + 8);
	std::vector<int>	iCardLst;

	if (!parseArgs(arg, iCardLst, table.getHand().size())) {
		return true;
	}
	for (int i = 0; i < iCardLst.size(); i ++)
		_playHand.push_back(_hand[iCardLst[i]]);
	_hand = removeCardWithIndexLst(_hand, iCardLst);
	return (false);
}

bool	select(Screen &screen, Table &table, Player &player, std::string response)
{
	if (table.selectHand(response, table))
			return false;
	pokerHand	ph = getPokerHand(table.getPlayHand());
	screen.clear();
	table.setTokens(pokerHandScore[ph][0]);
	table.setMult(pokerHandScore[ph][1]);
	screen.putTable(table, player);
	screen.putCardList(table.getPlayHand(), 35, 10, false);
	screen.putText(pokerHandStr[ph], 1, 12);
	return true;
}
