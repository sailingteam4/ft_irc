/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converter.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 14:15:01 by mbico             #+#    #+#             */
/*   Updated: 2025/04/24 15:29:30 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "error.hpp" 
#include <cerrno>
#include <cstdlib>


uint16_t	ft_atous(const char *str)
{
	char	*endptr;
	errno = 0;

	uint64_t	val = strtol(str, &endptr, 10);
	if (errno || *endptr || val > UINT16_MAX)
	{
		errno_irc = BAD_CONVERSION;
		return (0);
	}
	errno_irc = NO_ERROR;
	return (static_cast<uint16_t>(val));
}
