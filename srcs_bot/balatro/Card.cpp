/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Card.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 23:43:49 by mateo             #+#    #+#             */
/*   Updated: 2025/05/09 17:44:30 by mateo            ###   ########.fr       */
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

std::vector<std::string> makeLine(const char* elems[], size_t size) {
    std::vector<std::string> line;
    for (size_t i = 0; i < size; ++i) {
        line.push_back(elems[i]);
    }
    return line;
}

std::vector<std::vector<std::string> >	Card::getImg() const
{
	std::vector<std::vector<std::string> >	img;
	
	char val1[2] = { value_str[_value][0], '\0' };
	char val2[2] = { value_str[_value][1], '\0' };
	const char* card[5][7] = {
							{"╭", "─", "─", "─", "─", "─", "╮"},
							{"│", color_str[_color].c_str(), " ", " ", " ", " ", "│"},
							{"│", " ", " ", val1, val2, " ", "│"},
							{"│", " ", " ", " ", " ", color_str[_color].c_str(), "│"},
							{"╰", "─", "─", "─", "─", "─", "╯"}
	};

	std::cout << color_str[_color].c_str() << std::endl; 

	for (int i = 0; i < 5; i ++)
		img.push_back(makeLine(card[i], 7));
	return(img);
}

std::vector<std::vector<std::string> >	Card::getCovertImg() const
{
	std::vector<std::vector<std::string> >	img;
	
	char val1[2] = { value_str[_value][0], '\0' };
	char val2[2] = { value_str[_value][1], '\0' };
	const char* card[5][4] = {
							{"┈", "─", "─", "╮"},
							{" ", " ", " ", "│"},
							{val1, val2, " ", "│"},
							{" ", " ", color_str[_color].c_str(), "│"},
							{"┈", "─", "─", "╯"}
	};

	for (int i = 0; i < 5; i ++)
		img.push_back(makeLine(card[i], 4));
	return(img);
}


