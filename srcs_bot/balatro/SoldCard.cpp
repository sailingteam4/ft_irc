/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SoldCard.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 06:41:19 by mateo             #+#    #+#             */
/*   Updated: 2025/05/19 06:45:31 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SoldCard.hpp"
#include "Card.hpp"
#include "bot.hpp"

SoldCard::SoldCard() : Card(SOLD_CARD, 0) {}

std::vector<std::vector<std::string> >	SoldCard::getImg() const
{
	std::vector<std::vector<std::string> >	img;
	
	const char* card[5][7] = {
							{"╭", "─", "─", "─", "─", "─", "╮"},
							{"│", " ", " ", " ", " ", " ", "│"},
							{"│", " ", "S", "O", "L", "D", "│"},
							{"│", " ", " ", " ", " ", " ", "│"},
							{"╰", "─", "─", "─", "─", "─", "╯"}
	};

	for (int i = 0; i < 5; i ++)
		img.push_back(makeLine(card[i], 7));
	return(img);
}

SoldCard	*SoldCard::clone() const {
	return new SoldCard(*this);
}

