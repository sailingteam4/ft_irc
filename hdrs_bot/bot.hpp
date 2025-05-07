/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:38:43 by mbico             #+#    #+#             */
/*   Updated: 2025/05/07 00:02:17 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>
#include <string>
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define WHITE   "\033[37m"
#define BOLDMAGENTA "\033[1m\033[35m"

#define	SERVER "127.0.0.1"
#define	NAME "bot"
#define	CHANNEL	"#test"
#define	TAG	"!"

uint16_t	ft_atous(const char *str);
uint16_t	portValid(const char *strPort);
std::string	passwordValid(const char *password);
void		argValid(int argc);
int			commandHandle(int32_t sockfd);
int			randint(int min, int max);
