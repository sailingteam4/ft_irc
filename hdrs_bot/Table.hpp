/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Table.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:20:07 by mbico             #+#    #+#             */
/*   Updated: 2025/05/12 13:48:11 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Card.hpp"
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
	SHOP,
};

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

static const uint32_t	pokerHandScore[][2] = {
	{5, 1},
	{10, 2},
	{20, 2},
	{30, 3},
	{30, 4},
	{35, 4},
	{40, 4},
	{60, 7},
	{100, 8}
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

		uint32_t	_mult;
		uint32_t	_tokens;

		std::vector<Card>	_deck;
		std::vector<Card>	_playHand;
		std::vector<Card>	_hand;
	public :
		Table(Player player);
		~Table(void);
		void	nextRound(uint32_t round);
		void	firtHand(Player player);
		void	addCardtoHand(Player player, uint32_t nb);

		std::vector<Card>	getHand() const;
		std::vector<Card>	getPlayHand() const;

		uint32_t			getMult() const;
		uint32_t			getTokens() const;
		void				setMult(uint32_t nb);
		void				setTokens(uint32_t nb);
		void				addMult(uint32_t nb);
		void				addTokens(uint32_t nb);

		void				calculateUserScore(Player player);

		void				removeDiscardRemains();
		void				resetDiscardRemains(Player player);
		void				removeHandRemains();
		void				resetHandRemains(Player player);
		
		uint32_t			getRound() const;
		uint32_t			getWinScore() const;
		uint32_t			getUserScore() const;
		uint32_t			getHandRemains() const;
		uint32_t			getDiscardRemains() const;

		void				playHandClear();
		void				handClear();

		bool	selectHand(std::string response, Table table);
		void	insertPlayCardinHand();
};

std::vector<Card>	sortValue(std::vector<Card> cards);
pokerHand			getPokerHand(std::vector<Card> hand);
void				handValue(Table &table, std::vector<Card> hand, pokerHand ph);
