/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Card.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 23:43:49 by mateo             #+#    #+#             */
/*   Updated: 2025/05/07 23:43:30 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Card.hpp"
#include "color.hpp"
#include "bot.hpp"
#include "Screen.hpp"
#include <iostream>

Card::Card(uint8_t value, e_color color)
{
	_value = value;
	_color = color;
	_tokens = value;
	if (value >= JACK && value <= KING)
		_tokens = 10;
	else if (value == AS)
		_tokens = 11;

	_edition = NO_EDITION;
	_enhancement = NO_ENHANCEMENT;
	_seal = NO_SEAL;
	std::cout << "[System]: Card value=" << (int)_value << ", color=" << _color << ", tokens=" << _tokens << " created" << std::endl;
}

Card::Card()
{
	_value = 0;
	_color = SPIKE;
	_tokens = 0;
	_edition = NO_EDITION;
	_enhancement = NO_ENHANCEMENT;
	_seal = NO_SEAL;
}


Card	&Card::operator=(const Card &cpy)
{
	if (this != &cpy)
	{
		_value = cpy._value;
		_color = cpy._color;
		_tokens = cpy._tokens;
		_edition = cpy._edition;
		_enhancement = cpy._enhancement;
		_seal = cpy._seal;
	}
	return (*this);
}

Card::Card(const Card &cpy)
{
	*this = cpy;
}

Card::~Card()
{
}

uint8_t			Card::getValue() const {
	return (_value);
}

e_color			Card::getColor() const {
	return (_color);
}

uint32_t		Card::getTokens() const {
	return (_tokens);
}

e_edition		Card::getEdition() const {
	return (_edition);
}

e_enhancement	Card::getEnhancement() const {
	return (_enhancement);
}

e_seal			Card::getSeal() const {
	return (_seal);
}

void	Card::setCardDefault()
{
	_value = 0;
	_color = SPIKE;
	_tokens = 0;
	_edition = NO_EDITION;
	_enhancement = NO_ENHANCEMENT;
	_seal = NO_SEAL;
}

std::vector<std::string>	Card::getImg() const
{
	std::vector<std::string>	img;

	img.push_back(CARD_TOP);
	img.push_back(CARD_ICON(color_str[_color]));
	img.push_back(CARD_MID(value_str[_value]));
	img.push_back(CARD_RICON(color_str[_color]));
	img.push_back(CARD_BTM);

	return(img);
}
