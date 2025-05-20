/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 21:53:20 by mbico             #+#    #+#             */
/*   Updated: 2025/05/20 15:33:59 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>
#include "PlayingCard.hpp"
#include "PlanetCard.hpp"

class Player {
	private :
		int32_t				_money;
		uint8_t				_handSize;
		uint8_t				_handMax;
		uint8_t				_discardMax;
		uint32_t			_deckSize;
		std::vector<PlayingCard>	_deck;
		std::vector<std::vector<uint32_t > >	_pokerHandScore;

		std::vector<Card *>	_consomCard;
	public :
		Player(void);
		~Player(void);

		uint32_t	getMoney() const;
		uint8_t		getHandSize() const;
		uint8_t		getHandMax() const;
		uint8_t		getDiscardMax() const;
		uint32_t	getDeckSize() const;

		void		addMoney(int32_t nb);
		void		setMoney(int32_t nb);
		void		addConsomCard(Card *card);
		
		std::vector<Card *>			getConsomCard() const;
		std::vector<PlayingCard>	getDeck() const;
		std::vector<std::vector<uint32_t > >	getPokerHandScore() const;
		void						addPokerHandLevel(pokerHand ph, uint32_t token, uint32_t mult);
		void						eraseConsomCard(uint32_t ind);

		void	clearConsom();
		void	free();
};
