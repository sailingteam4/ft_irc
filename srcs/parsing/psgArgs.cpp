/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   psgArgs.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 18:52:48 by mateo             #+#    #+#             */
/*   Updated: 2025/04/17 17:21:44 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.hpp"
#include "utils.hpp"
#include "ft_irc.hpp"

uint16_t	portValid(const char *strPort)
{
	uint16_t	port;

	port = ft_atous(strPort);
	if (errno_irc || port <= 1024)
		throw errPsg::invalidPortException();
	return (port);
}

std::string	passwordValid(const char *password)
{
    std::string pwStr(password);

    if (pwStr.empty())
		throw errPsg::invalidPasswordException();
	return (pwStr);
}

void	argValid(int argc)
{
	if (argc != 3)
		throw errPsg::invalidArgException();
}
