/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:38:43 by mbico             #+#    #+#             */
/*   Updated: 2025/05/20 18:02:48 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>
#include <string>
#include <iostream>
#include <string>
#include <cstring>
#include <sys/select.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "PlayingCard.hpp"
#include "Table.hpp"
#include "Screen.hpp"

#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define WHITE   "\033[37m"
#define BOLDMAGENTA "\033[1m\033[35m"

#define	SERVER "127.0.0.1"
#define	NAME "bot"
#define	CHANNEL	"#balatro"
#define	TAG	"!"
static const std::string	privmsg = "PRIVMSG " CHANNEL " :";
extern	bool stopServer;

uint16_t	ft_atous(const char *str);
uint16_t	portValid(const char *strPort);
std::string	passwordValid(const char *password);
void		argValid(int argc);
int			commandHandle(int32_t sockfd, fd_set master);
int			randint(int min, int max);
std::string	intToString(int n);
int			countCardOcc(std::vector<PlayingCard> hand, int val);
void		gameHandle(int sockfd, fd_set master);
std::string	getResponse(int sockfd, fd_set master);
bool isNumber(const std::string& s);

std::vector<std::string> makeLine(const char* elems[], size_t size);

bool	pay(Screen &screen, Table &table, Player &player, std::string response);
bool		selectc(Screen &screen, Table &table, Player &player, std::string response);
void		deselect(Screen &screen, Table &table, Player &player);
void		play(Screen &screen, Table &table, Player &player);
void		discard(Screen &screen, Table &table, Player &player);
bool		use(Table &table, Player &player, std::string response);
