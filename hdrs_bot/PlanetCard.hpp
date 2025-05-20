/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PlanetCard.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 21:28:47 by mateo             #+#    #+#             */
/*   Updated: 2025/05/19 07:37:32 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include "Card.hpp"

enum	pokerHand {
	HIGH_CARD,
	PAIR,
	TWO_PAIR,
	THREE_OF_A_KIND,
	STRAIGHT,
	FLUSH,
	FULL_HOUSE,
	FOUR_OF_A_KIND,
	STRAIGHT_FLUSH,
} ;

static const uint32_t	planetAddScore[9][2] = {
	{1, 10},
	{1, 15},
	{1, 20},
	{2, 20},
	{2, 25},
	{2, 15},
	{2, 25},
	{3, 30},
	{3,40}
};

static const std::string	planetName[9] = {"Plu", "Mer", "Ura", "Ven", "Sat", "Jup", "Ear", "Mar", "Nep"};
static const std::string	planetShem[9] = {"A....", "AA...", "AAKK.", "AAA..", "A2345", "#####", "KKAAA", "AAAA.", "A2345"};


class	PlanetCard : public Card{
	private:
		std::string	_name;
		pokerHand	_pokerHand;
		uint32_t	_mult;
		uint32_t	_token;
	public:
		PlanetCard();
		PlanetCard(uint32_t pkh);
		PlanetCard(const PlanetCard &cpy);
		PlanetCard	&operator=(const PlanetCard &cpy);
		~PlanetCard();

		PlanetCard *clone() const;
		std::string	getName() const;
		std::vector<std::vector<std::string> > getImg() const;

		pokerHand	getPokerHand() const;
		uint32_t	getMult() const;
		uint32_t	getToken() const;
};

