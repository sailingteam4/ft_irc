/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Card.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 01:19:22 by mateo             #+#    #+#             */
/*   Updated: 2025/05/19 06:54:28 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Card.hpp"
#include "bot.hpp"

Card::Card(e_cardtype type, uint32_t price) {
	_type = type;
	_price = price;
}

e_cardtype	Card::getType() const {
	return (_type);
}

uint32_t	Card::getPrice() const {
	return (_price);
}


