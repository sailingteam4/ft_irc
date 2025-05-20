/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SoldCard.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 06:41:43 by mateo             #+#    #+#             */
/*   Updated: 2025/05/19 06:44:43 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Card.hpp"

class	SoldCard : public Card {
	public:
		SoldCard();
		~SoldCard() {}

		SoldCard *clone() const;
		std::vector<std::vector<std::string > > getImg() const;
};
