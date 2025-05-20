/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Table.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:20:07 by mbico             #+#    #+#             */
/*   Updated: 2025/05/20 18:00:35 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "PlayingCard.hpp"
#include "Player.hpp"
#include <stdint.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <list>

enum	gameStatus {
	NOGAME,
	TABLE_S,
	SELECT_CARD,
	WIN_SCREEN,
	SHOP,
};

static const std::string	pokerHandStr[] = {"High Card", "Pair", "Two Pair", "Three of a Kind", "Straight", "Flush", "Full House", "Four of a Kind", "Straight Flush"};

static const uint64_t	initBet[][3] = {{100, 150, 200},\
				{300, 450, 600},\
				{800, 1200, 1600},\
				{2000, 3000, 4000},\
				{5000, 7500, 10000},\
				{11000, 16500, 22000},\
				{20000, 30000, 40000},\
				{35000, 52500, 70000},\
				{50000, 75000, 100000},\
				{110000, 165000, 220000},\
				{560000, 840000, 1120000},\
				{7200000, 10800000, 14400000},\
				{300000000, 450000000, 600000000},\
				{47000000000, 70500000000, 94000000000},
};

class	Table {
	private :
		uint32_t	_round;
		uint32_t	_winScore;
		uint32_t	_userScore;
		uint32_t	_handRemains;
		uint32_t	_discardRemains;
		uint32_t	_rollCost;

		uint32_t	_reward;

		uint32_t	_mult;
		uint32_t	_tokens;

		std::vector<PlayingCard>	_deck;
		std::vector<PlayingCard>	_playHand;
		std::vector<PlayingCard>	_hand;

		std::vector<Card *>			_shop;
		std::vector<PlanetCard>	_remainPlanet;
	public :
		Table(Player player);
		~Table(void);
		void	nextRound(Player player);
		void	firtHand(Player player);
		void	addCardtoHand(uint32_t nb);

		std::vector<PlayingCard>	getHand() const;
		std::vector<PlayingCard>	getPlayHand() const;

		uint32_t			getMult() const;
		uint32_t			getTokens() const;
		void				setMult(uint32_t nb);
		void				setTokens(uint32_t nb);
		void				addMult(uint32_t nb);
		void				addTokens(uint32_t nb);
		void				setRollCost(uint32_t nb);
		void				addRollCost(uint32_t nb);

		void				calculateUserScore();

		void				removeDiscardRemains();
		void				resetDiscardRemains(Player player);
		void				removeHandRemains();
		void				resetHandRemains(Player player);
		void				updateReward();
		void				soldShopCard(uint32_t ind);
		
		uint32_t			getRound() const;
		uint32_t			getWinScore() const;
		uint32_t			getUserScore() const;
		uint32_t			getHandRemains() const;
		uint32_t			getDiscardRemains() const;
		uint32_t			getReward() const;
		std::vector<Card *>			getShop() const;
		uint32_t			getRollCost() const;


		void				playHandClear();
		void				handClear();

		bool	selectHand(std::string response, Table table);
		void	insertPlayCardinHand();
		void	rollShop();
		void	addRemainPlanet(PlanetCard pc);

		void	clearShop();
		void	free();
};

std::vector<PlayingCard>	sortValue(std::vector<PlayingCard> cards);
pokerHand			getPokerHand(std::vector<PlayingCard> hand);
void				handValue(Table &table, std::vector<PlayingCard> hand, pokerHand ph);
