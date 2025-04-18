/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrontey <nrontey@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:57:01 by mbico             #+#    #+#             */
/*   Updated: 2025/04/18 01:00:41 by nrontey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>
#include <iostream>

#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define WHITE   "\033[37m"
#define BOLDMAGENTA "\033[1m\033[35m"

#define SERVER_NAME "feur.apagnan.bebou"

void		argValid(int argc);
uint16_t	portValid(const char *strPort);
std::string	passwordValid(const char *password);
