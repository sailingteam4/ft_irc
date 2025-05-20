/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pokerHand.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 02:23:29 by mateo             #+#    #+#             */
/*   Updated: 2025/05/20 17:51:21 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PlayingCard.hpp"
#include "Table.hpp"
#include "bot.hpp"


bool	isFiveofaKind(std::vector<PlayingCard> hand)
{
	if ((int)hand.size() != 5)
		return true;
	for (int i = 0; i < 5; i ++) {
		if (hand[0].getValue() != hand[i].getValue())
			return (false);
	}
	return (true);
}

bool	isFlush(std::vector<PlayingCard> hand)
{
	if ((int)hand.size() != 5)
		return false;
	for (int i = 0; i < 5; i ++) {
		if (hand[0].getColor() != hand[i].getColor())
			return (false);
	}
	return (true);
}

bool	isStraight(std::vector<PlayingCard> hand)
{
	if ((int)hand.size() != 5)
		return (false);
	if (hand[0].getValue() == AS &&
		hand[1].getValue() == KING &&
		hand[2].getValue() == QUEEN &&
		hand[3].getValue() == JACK &&
		hand[4].getValue() == 10)
		return true;
	for (int i = 0; i < 4; ++i) {
		if (hand[i + 1].getValue() != hand[i].getValue() + 1)
			return false;
	}
	return true;
}

bool	isStraightFlush(std::vector<PlayingCard> hand)
{
	if (isStraight(hand) && isFlush(hand))
		return true;
	return false;
}



bool	isFullHouse(std::vector<PlayingCard> hand)
{
	if ((int)hand.size() != 5)
		return (false);
	int	occ1 = countCardOcc(hand, hand[0].getValue());
	int	occ2 = countCardOcc(hand, hand[3].getValue());
	if ((occ1 == 2 && occ2 == 3) || (occ1 == 3 && occ2 == 2))
		return true;
	return (false);
}

bool isTwoPair(std::vector<PlayingCard> hand) {
	if ((int)hand.size() < 4)
		return false;

	int pairCount = 0;
	int i = 0;

	while (i < (int)hand.size() - 1)
	{
		if (hand[i].getValue() == hand[i + 1].getValue())
		{
			pairCount++;
			i += 2;
		} 
		else
			i++;
	}
	return (pairCount == 2);
}

bool	isFourofaKind(std::vector<PlayingCard> hand)
{
	if ((int)hand.size() < 4)
		return (false);
	for (int i = 0; i < (int)hand.size(); i ++)
	{
		if (countCardOcc(hand, hand[i].getValue()) == 4)
			return true;
	}
	return false;
}

bool	isThreeofaKind(std::vector<PlayingCard> hand)
{
	if ((int)hand.size() < 3)
		return (false);
	for (int i = 0; i < (int)hand.size(); i ++)
	{
		if (countCardOcc(hand, hand[i].getValue()) == 3)
			return true;
	}
	return false;
}

bool	isPair(std::vector<PlayingCard> hand)
{
	if ((int)hand.size() < 2)
		return (false);
	for (int i = 0; i < (int)hand.size(); i ++)
	{
		if (countCardOcc(hand, hand[i].getValue()) == 2)
			return true;
	}
	return false;
}

bool	isFlushHouse(std::vector<PlayingCard> hand)
{
	if ((int)hand.size() != 5)
		return false;
	if (isFullHouse(hand) && isFlush(hand))
		return true;
	return(false);
}

bool	isFlushFive(std::vector<PlayingCard> hand)
{
	if ((int)hand.size() != 5)
		return false;
	if (isFiveofaKind(hand) && isFlush(hand))
		return true;
	return(false);
}

pokerHand	getPokerHand(std::vector<PlayingCard> hand)
{
	if ((int)hand.size() > 5)
		return HIGH_CARD;

	hand = sortValue(hand);
	if (isStraightFlush(hand))
		return (STRAIGHT_FLUSH);
	else if (isFourofaKind(hand))
		return (FOUR_OF_A_KIND);
	else if (isFullHouse(hand))
		return (FULL_HOUSE);
	else if (isFlush(hand))
		return (FLUSH);
	else if (isStraight(hand))
		return (STRAIGHT);
	else if (isThreeofaKind(hand))
		return (THREE_OF_A_KIND);
	else if (isTwoPair(hand))
		return (TWO_PAIR);
	else if (isPair(hand))
		return (PAIR);
	return (HIGH_CARD);
}





