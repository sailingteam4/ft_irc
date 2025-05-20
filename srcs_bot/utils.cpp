/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 23:59:18 by mateo             #+#    #+#             */
/*   Updated: 2025/05/20 18:05:36 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <ctime>
#include <string>
#include <stdint.h>
#include <sstream>
#include "PlayingCard.hpp"
#include "bot.hpp"
#include <cstring>
#include <arpa/inet.h>
#include <sys/select.h>

int	randint(int min, int max) 
{
	return (min + rand() % (max - min + 1));
}

std::string intToString(int n) {
    std::stringstream ss;
    ss << n;
    return ss.str();
}

int	countCardOcc(std::vector<PlayingCard> hand, int val) {
	int	nb = 0;

	for (int i = 0; i < (int)hand.size(); i ++)
	{
		if (hand[i].getValue() == val)
			nb ++;
	}
	return nb;
}


std::string	getResponse(int sockfd, fd_set master)
{
	char buffer[512];
	int bytes_received;

	memset(buffer, 0, sizeof(buffer));

	if (select(sockfd + 1, &master, NULL, NULL, NULL) <= 0)
		return "";
	bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0)
		return "";
    std::string response(buffer);
	return (response);
}
#include <iostream>
bool isNumber(const std::string& s) {
	if (s.empty()) return false;
	
	std::size_t i = 0;
	if (s[0] == '-' || s[0] == '+') i++;
	for (; i < s.size(); ++i)
		if (!std::isdigit(s[i]) && s[i] >= 32)
			return false;
	return true;
}


