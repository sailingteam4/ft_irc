/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 02:10:25 by mateo             #+#    #+#             */
/*   Updated: 2025/05/07 02:21:15 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Table.hpp>
#include <algorithm>

bool	compareValue(const Card &a, const Card &b)
{
	int	ax = a.getValue();
	int	bx = b.getValue();

	if (ax == 1)
		ax = 20;
	if (bx == 1)
		bx = 20;
	return (ax > bx);
}

bool	compareColor(const Card &a, const Card &b)
{
	return (a.getColor() > b.getColor());
}

std::vector<Card>	sortValue(std::vector<Card> cards)
{
	std::sort(cards.begin(), cards.end(), compareValue);
	return (cards);
}
