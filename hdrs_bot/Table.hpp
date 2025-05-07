/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Table.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:20:07 by mbico             #+#    #+#             */
/*   Updated: 2025/05/07 21:20:22 by mbico            ###   ########.fr       */
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

		void	displayHand(int sockfd) const;

		std::vector<Card>	getHand();
};


std::vector<Card>	sortValue(std::vector<Card> cards);
