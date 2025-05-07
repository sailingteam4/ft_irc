/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Screen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 21:55:13 by mbico             #+#    #+#             */
/*   Updated: 2025/05/08 00:20:17 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Screen.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include "bot.hpp"

Screen::Screen() 
{
	_lenx = 87;
	_leny = 24;
	for (uint32_t y = 0; y < _leny; y ++)
	{
		std::string	line = "";
		for (uint32_t x = 0; x < _lenx; x ++)
			line += ".";
		_screen.push_back(line);
	}
}

Screen::~Screen(void) {
}

std::vector<std::string>	Screen::getScreen(void) const {
	return(_screen);
}

uint32_t	Screen::getLenX() const {
	return(_lenx);
}

uint32_t	Screen::getLenY() const {
	return (_leny);
}

void	Screen::printScreen(void) const
{
	for (uint32_t y = 0; y < _leny; y ++)
	{
		std::cout << _screen[y] << std::endl;
	}
}

void	Screen::displayScreen(int sockfd) const
{
	for (uint32_t y = 0; y < _leny; y ++)
	{
		std::string	str = privmsg + _screen[y] + "\r\n";
		send(sockfd, str.c_str(), str.length(), 0);
	}
}

void	Screen::putImage(std::vector<std::string> img, uint32_t x, uint32_t y)
{
	uint32_t	offset;

	if (x >= _lenx || y >= _leny || y + img.size() >= _leny || x + img[0].size() >= _lenx)
		return;
	for (int yr = 0; yr < (int)img.size(); yr ++)
	{
		int	xr;
		for (xr = 0; xr < (int)img[yr].size(); xr ++)
		{
			_screen[y + yr][x + xr] = img[yr][xr];
		}
		/* offset = countSymbole(img[yr]); */
		/* for (int i = 0; i < offset; i ++) */
		/* 	_screen[y + yr].insert(x + xr + i, BACKGROUND); */
	}
	for (int i = 0; i < (int)_screen.size(); i ++)
		std::cout << _screen[i] << std::endl;
}
