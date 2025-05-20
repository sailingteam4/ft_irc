/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PlayingCard.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 23:43:49 by mateo             #+#    #+#             */
/*   Updated: 2025/05/19 03:49:44 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PlayingCard.hpp"
#include "color.hpp"
#include "bot.hpp"
#include "Screen.hpp"
#include <iostream>

PlayingCard::PlayingCard(uint8_t value, e_color color) : Card(PLAYING_CARD, 1)
{
	_value = value;
	_color = color;
	_tokens = value;
	if (value >= JACK && value <= KING)
		_tokens = 10;
	else if (value == AS)
		_tokens = 11;
	_type = PLAYING_CARD;
	_edition = NO_EDITION;
	_enhancement = NO_ENHANCEMENT;
	_seal = NO_SEAL;
	std::cout << "[System]: Card value=" << (int)_value << ", color=" << _color << ", tokens=" << _tokens << " created" << std::endl;
}

PlayingCard::PlayingCard() : Card(PLAYING_CARD, 1)
{
	_value = 0;
	_color = SPIKE;
	_tokens = 0;
	_edition = NO_EDITION;
	_enhancement = NO_ENHANCEMENT;
	_seal = NO_SEAL;
}


PlayingCard	&PlayingCard::operator=(const PlayingCard &cpy)
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

PlayingCard::PlayingCard(const PlayingCard &cpy) : Card(PLAYING_CARD, 1)
{
	*this = cpy;
}

PlayingCard::~PlayingCard()
{
}

uint8_t			PlayingCard::getValue() const {
	return (_value);
}

e_color			PlayingCard::getColor() const {
	return (_color);
}

uint32_t		PlayingCard::getTokens() const {
	return (_tokens);
}

e_edition		PlayingCard::getEdition() const {
	return (_edition);
}

e_enhancement	PlayingCard::getEnhancement() const {
	return (_enhancement);
}

e_seal			PlayingCard::getSeal() const {
	return (_seal);
}

void	PlayingCard::setCardDefault()
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

std::vector<std::vector<std::string> >	PlayingCard::getImg() const
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

	for (int i = 0; i < 5; i ++)
		img.push_back(makeLine(card[i], 7));
	return(img);
}

std::vector<std::vector<std::string> >	PlayingCard::getCovertImg() const
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

PlayingCard	*PlayingCard::clone() const {
	return new PlayingCard(*this);
}
