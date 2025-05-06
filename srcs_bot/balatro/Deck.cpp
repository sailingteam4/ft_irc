/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Deck.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 00:06:03 by mateo             #+#    #+#             */
/*   Updated: 2025/05/06 02:33:20 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Card.hpp"
#include "color.hpp"
#include <iostream>

Card	*defaultDeck()
{
	Card	*cards = cardList(53);

	for  (uint8_t i = 0; i < 4; i ++)
	{
		for (uint8_t j = 1; j < 14; j ++)
		{
			cards[i * 13 + j] = Card(j, (e_color)i);
		}
	}
	std::cout << YELLOW << "[System]: Default Deck created" << RESET << std::endl;
	return (cards);
}


