/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrontey <nrontey@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:57:01 by mbico             #+#    #+#             */
/*   Updated: 2025/05/05 22:32:47 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

extern	bool stopServer;

#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define WHITE   "\033[37m"
#define BOLDMAGENTA "\033[1m\033[35m"

#define SERVER_NAME "feur.apagnan.bebou"
#define DCC_IP "127.0.0.1"
#define DCC_PORT "0"

void		argValid(int argc);
uint16_t	portValid(const char *strPort);
std::string	passwordValid(const char *password);
