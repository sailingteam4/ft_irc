/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pokerHand.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 02:23:29 by mateo             #+#    #+#             */
/*   Updated: 2025/05/07 17:42:27 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Card.hpp"
#include "Table.hpp"


bool	isFiveofaKind(std::vector<Card> hand)
{
	if (hand.size() != 5)
		return (false);
	for (int i = 0; i < 5; i ++) {
		if (hand[0].getValue() != hand[i].getValue())
			return (false);
	}
	return (true);
}

/* bool	isFourofaKind(std::vector<Card> hand) */
/* { */
/* 	if (hand.size() < 4) */
/* 		return (false); */
/* 	for (int i <) */
/* } */

bool	isFlush(std::vector<Card> hand)
{
	if (hand.size() != 5)
		return (false);
	for (int i = 0; i < 5; i ++) {
		if (hand[0].getColor() != hand[i].getColor())
			return (false);
	}
	return (true);
}


