/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PlanetCard.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 21:34:50 by mateo             #+#    #+#             */
/*   Updated: 2025/05/19 07:38:59 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PlanetCard.hpp"
#include "Card.hpp"
#include "PlayingCard.hpp"
#include "Table.hpp"
#include "bot.hpp"

PlanetCard::PlanetCard() : Card(PLANET_CARD, 3) {
	_name = "default";
	_pokerHand = HIGH_CARD;
	_mult = 0;
	_token = 0;
}

PlanetCard::PlanetCard(uint32_t pkh) : Card(PLANET_CARD, 3) {
	_pokerHand = (pokerHand)pkh;
	_mult = planetAddScore[pkh][0];
	_token = planetAddScore[pkh][1];
	_name = planetName[pkh];
}

PlanetCard	&PlanetCard::operator=(const PlanetCard &cpy)
{
	if (this != &cpy)
	{
		_name = cpy._name;
		_pokerHand = cpy._pokerHand;
		_mult = cpy._mult;
		_token = cpy._token;
		_price = _price;
		_type = _type;
	}
	return (*this);
}

PlanetCard::PlanetCard(const PlanetCard &cpy) : Card(PLANET_CARD, 3) {
	*this = cpy;
}

PlanetCard::~PlanetCard() {}

std::vector<std::vector<std::string> >	PlanetCard::getImg() const
{
	std::vector<std::vector<std::string> >	img;
	
	char nm[][2] = {{planetName[_pokerHand][0], '\0' },
				 {planetName[_pokerHand][1], '\0' },
				 {planetName[_pokerHand][2], '\0' }};
	char	shem[][2] = {{planetShem[_pokerHand][0], '\0'},
				{planetShem[_pokerHand][1], '\0'},
				{planetShem[_pokerHand][2], '\0'},
				{planetShem[_pokerHand][3], '\0'},
				{planetShem[_pokerHand][4], '\0'},
	};

	const char* card[5][7] = {
							{"╭", "─", "─", "─", "─", "─", "╮"},
							{"│", " ", nm[0], nm[1], nm[2], " ", "│"},
							{"│", " ", " ", "○", " ", " ", "│"},
							{"│", shem[0], shem[1], shem[2], shem[3], shem[4], "│"},
							{"╰", "─", "─", "─", "─", "─", "╯"}
	};

	for (int i = 0; i < 5; i ++)
		img.push_back(makeLine(card[i], 7));
	return(img);
}

PlanetCard	*PlanetCard::clone() const {
	return new PlanetCard(*this);
}

std::string	PlanetCard::getName() const {
	return (_name);
}

pokerHand	PlanetCard::getPokerHand() const {
	return (_pokerHand);
}

uint32_t	PlanetCard::getMult() const {
	return (_mult);
}

uint32_t	PlanetCard::getToken() const {
	return (_token);
}
