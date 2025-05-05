/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Table.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 20:33:22 by mbico             #+#    #+#             */
/*   Updated: 2025/05/05 21:51:34 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Table.hpp"
#include "iostream"

Table::Table(void)
{
	std::cout << "New table create" << std::endl;
	_round = 0;
	_winScore = 800;
	_userScore = 0;
	_cardRemains = 52;
	_handRemains = 4;
	_discardRemains = 4;
}

Table::~Table(void)
{
	std::cout << "Table close" << std::endl;
}

void	Table::nextRound(uint32_t round)
{
	_round = round;
	_winScore = initBet[(round + 3) / 3][(round + 3) % 3];
	_userScore = 0;
	//autre truc
	
}
