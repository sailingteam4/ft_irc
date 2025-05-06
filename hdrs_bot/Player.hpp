/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 21:53:20 by mbico             #+#    #+#             */
/*   Updated: 2025/05/06 02:39:09 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>
#include "Card.hpp"

class Player {
	private :
		uint32_t	_money;
		uint8_t		_handSize;
		uint8_t		_handMax;
		uint8_t		_discardMax;
		uint32_t	_deckSize;
		Card		*_deck;


	public :
		Player(void);
		~Player(void);

		uint32_t	getMoney() const;
		uint8_t		getHandSize() const;
		uint8_t		getHandMax() const;
		uint8_t		getDiscardMax() const;
		uint32_t	getDeckSize() const;
		Card		*getDeck() const;
};
