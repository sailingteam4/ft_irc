/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Deck.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 00:06:03 by mateo             #+#    #+#             */
/*   Updated: 2025/05/19 00:59:05 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PlayingCard.hpp"
#include "color.hpp"
#include <iostream>
#include <vector>

std::vector<PlayingCard>	defaultDeck()
{
	std::vector<PlayingCard>	cards;

	for  (uint8_t i = 0; i < 4; i ++)
	{
		for (uint8_t j = 1; j < 14; j ++)
		{
			cards.push_back(PlayingCard(j, (e_color)i));
		}
	}
	std::cout << YELLOW << "[System]: Default Deck created" << RESET << std::endl;
	return (cards);
}


