/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 21:53:38 by mbico             #+#    #+#             */
/*   Updated: 2025/05/06 23:44:19 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Player.hpp"
#include "color.hpp"
#include <iostream>

Player::Player()
{
	_money = 4;
	_handSize = 8;
	_handMax = 4;
	_discardMax = 4;
	_deck = defaultDeck();
	std::cout << YELLOW << "[System]: Player class created" << RESET << std::endl;
}

Player::~Player()
{
	std::cout << YELLOW << "[System]: Player class deleted" << RESET << std::endl;
}

uint32_t	Player::getMoney() const {
	return (_money);
}

uint8_t		Player::getHandSize() const {
	return (_handSize);
}

uint8_t		Player::getHandMax() const {
	return (_handMax);
}

uint8_t		Player::getDiscardMax() const {
	return (_discardMax);
}

std::vector<Card>	Player::getDeck() const {
	return (_deck);
}

