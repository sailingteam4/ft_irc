/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 23:59:18 by mateo             #+#    #+#             */
/*   Updated: 2025/05/10 10:38:53 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <ctime>
#include <string>
#include <stdint.h>
#include <sstream>
#include "Card.hpp"
#include <cstring>
#include <arpa/inet.h>

int	randint(int min, int max) 
{
	return (min + rand() % (max - min + 1));
}

std::string intToString(int n) {
    std::stringstream ss;
    ss << n;
    return ss.str();
}

int	countCardOcc(std::vector<Card> hand, int val) {
	int	nb = 0;

	for (int i = 0; i < hand.size(); i ++)
	{
		if (hand[i].getValue() == val)
			nb ++;
	}
	return nb;
}


std::string	getResponse(int sockfd)
{
	char buffer[512];
	int bytes_received;

	memset(buffer, 0, sizeof(buffer));
	bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0)
		return "";
    std::string response(buffer);
	return (response);
}
