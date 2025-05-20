/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Table.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:33:22 by mbico             #+#    #+#             */
/*   Updated: 2025/05/20 18:00:49 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Table.hpp"
#include "Card.hpp"
#include "PlayingCard.hpp"
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
	_reward = 3;
	_handRemains = player.getHandMax();
	_discardRemains = player.getDiscardMax();	for (uint32_t i = 0; i < 9; i ++)
		_remainPlanet.push_back(PlanetCard(i));

	_rollCost = 5;

	_mult = 0;
	_tokens = 0;

	_deck = player.getDeck();
}

Table::~Table(void)
{
	std::cout << YELLOW << "[System]: Table deleted" << RESET << std::endl;
}

void	Table::nextRound(Player player)
{
	_round ++;
	_winScore = initBet[(_round + 2) / 3][(_round + 2) % 3];
	_userScore = 0;
	_handRemains = player.getHandMax();
	_discardRemains = player.getDiscardMax();
	_mult = 0;
	_tokens = 0;
	_deck = player.getDeck();
	_rollCost = 5;
	_hand.clear();
	_playHand.clear();
	this->updateReward();
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

void	Table::addCardtoHand(uint32_t nb)
{
	int	randi;

	for (int i = 0; i < (int)nb; i++)
	{
		randi = randint(0, _deck.size() - 1);
		_hand.push_back(_deck[randi]);
		_deck.erase(_deck.begin() + randi);
	}
	_hand = sortValue(_hand);

}

std::vector<PlayingCard>	Table::getHand() const {
	return (_hand);
}

std::vector<PlayingCard>	Table::getPlayHand() const {
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

uint32_t	Table::getReward() const {
	return (_reward);
}

uint32_t	Table::getRollCost() const {
	return (_rollCost);
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

void	Table::calculateUserScore()
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

void	Table::updateReward() {
	_reward = 5 - (_round % 3);
}

void	Table::addRollCost(uint32_t nb) {
	_rollCost += nb;
}

void	Table::setRollCost(uint32_t nb) {
	_rollCost = nb;
}

void	Table::addRemainPlanet(PlanetCard pc)
{
	_remainPlanet.push_back(pc);
}

void	Table::rollShop() {
	for (int i = 0; i < (int)_shop.size(); i ++)
	{
		if (_shop[i]->getType() == PLANET_CARD)
		{
			PlanetCard	*pc = (PlanetCard *)(_shop[i])->clone();
			_remainPlanet.push_back(*pc);
			delete pc;
		}
		delete _shop[i];
	}
	_shop.clear();
	for (int j = 0; j < 2; j ++)
	{
		uint32_t	randi = randint(0, _remainPlanet.size() - 1);
		_shop.push_back(new PlanetCard(_remainPlanet[randi]));
		_remainPlanet.erase(_remainPlanet.begin() + randi);
	}
}

std::vector<Card *>	Table::getShop() const {
	return(_shop);
}

void Table::clearShop() {
    for (size_t i = 0; i < _shop.size(); ++i) {
        delete _shop[i];
    }
    _shop.clear();
}

void	Table::free() {
	this->clearShop();
}
