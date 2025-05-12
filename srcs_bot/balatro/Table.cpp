/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Table.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:33:22 by mbico             #+#    #+#             */
/*   Updated: 2025/05/12 13:48:10 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Table.hpp"
#include "Card.hpp"
#include "color.hpp"
#include "bot.hpp"
#include <iostream>

Table::Table(Player player)
{
	std::cout << YELLOW << "[System]: Table created" << RESET << std::endl;
	srand(time(0));
	_round = 1;
	_winScore = 300;
	_userScore = 0;
	_handRemains = player.getHandMax();
	_discardRemains = player.getDiscardMax();

	_mult = 0;
	_tokens = 0;

	_deck = player.getDeck();
}

Table::~Table(void)
{
	std::cout << YELLOW << "[System]: Table deleted" << RESET << std::endl;
}

void	Table::nextRound(uint32_t round)
{
	_round = round;
	_winScore = initBet[(round + 2) / 3][(round + 2) % 3];
	_userScore = 0;
	//autre truc
}

void	Table::firtHand(Player player)
{
	int	randi;

	_hand.clear();
	for (int i = 0; i < player.getHandSize(); i++)
	{
		randi = randint(0, _deck.size() - 1);
		_hand.push_back(_deck[randi]);
		_deck.erase(_deck.begin() + randi);
	}
	_hand = sortValue(_hand);
}

void	Table::addCardtoHand(Player player, uint32_t nb)
{
	int	randi;

	for (int i = 0; i < nb; i++)
	{
		randi = randint(0, _deck.size() - 1);
		_hand.push_back(_deck[randi]);
		_deck.erase(_deck.begin() + randi);
	}
	_hand = sortValue(_hand);

}

std::vector<Card>	Table::getHand() const {
	return (_hand);
}

std::vector<Card>	Table::getPlayHand() const {
	return (_playHand);
}

uint32_t	Table::getRound() const {
	return (_round);
}

uint32_t	Table::getWinScore() const {
	return (_winScore);
}

uint32_t	Table::getUserScore() const {
	return (_userScore);
}

uint32_t	Table::getHandRemains() const {
	return (_handRemains);
}

uint32_t	Table::getDiscardRemains() const {
	return (_discardRemains);
}

uint32_t	Table::getMult() const {
	return (_mult);
}

uint32_t	Table::getTokens() const {
	return (_tokens);
}

void	Table::setMult(uint32_t nb) {
	_mult = nb;
}

void	Table::setTokens(uint32_t nb) {
	_tokens = nb;
}

void	Table::addMult(uint32_t nb) {
	_mult += nb;
}

void	Table::addTokens(uint32_t nb) {
	_tokens += nb;
}

void	Table::playHandClear()
{
	_playHand.clear();
}

void	Table::handClear()
{
	_hand.clear();
}

void	Table::calculateUserScore(Player player)
{
	_userScore += _tokens * _mult;
}

void	Table::removeHandRemains()
{
	if (_handRemains > 0)
		_handRemains --;
}

void	Table::resetHandRemains(Player player) {
	_handRemains = player.getHandMax();
}

void	Table::removeDiscardRemains()
{
	if (_discardRemains > 0)
		_discardRemains --;
}

void	Table::resetDiscardRemains(Player player) {
	_discardRemains = player.getDiscardMax();
}
