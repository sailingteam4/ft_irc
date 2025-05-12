/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handValue.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:52:14 by mateo             #+#    #+#             */
/*   Updated: 2025/05/12 13:14:48 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Table.hpp"
#include "bot.hpp"

bool	highestCard(Card c, std::vector<Card> hand)
{
	for (int i = 0; i < hand.size(); i ++)
	{
		if (c.getValue() < hand[i].getValue() && c.getValue() != 1)
			return (false);
	}
	return (true);
}

bool	isCardBuff(Card c, std::vector<Card> hand, pokerHand ph)
{
	int	occ = countCardOcc(hand, c.getValue());

	if (ph == HIGH_CARD)
		return (highestCard(c, hand));
	else if (ph == PAIR || ph == TWO_PAIR)
		return (occ == 2);
	else if (ph == THREE_OF_A_KIND)
		return (occ == 3);
	else if (ph == FOUR_OF_A_KIND)
		return (occ == 4);
	return (true);
}

void	handValue(Table &table, std::vector<Card> hand, pokerHand ph)
{
	for (int i = 0; i < hand.size(); i ++)
	{
		if (isCardBuff(hand[i], hand, ph))
		{
			table.addTokens(hand[i].getTokens());
		}
	}
}
