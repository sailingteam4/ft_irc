/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 18:52:48 by mateo             #+#    #+#             */
/*   Updated: 2025/04/14 19:06:24 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.hpp"
#include "utils.hpp"

uint16_t	portValid(const char *strPort)
{
	uint16_t	port;

	port = ft_atous(strPort);
	if (errno_irc || port <= 1024)
		throw invalidPortException();
	return (port);
}
