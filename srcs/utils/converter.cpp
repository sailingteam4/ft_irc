/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   converter.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 14:15:01 by mbico             #+#    #+#             */
/*   Updated: 2025/04/03 16:22:19 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "exception.hpp" 
#include <cerrno>
#include <cstdlib>


uint16_t	convertCharToUint16(const char *str)
{
	char	*endptr;
	errno = 0;

	uint64_t	val = strtol(str, &endptr, 10);
	if (errno || *endptr || val <= 1024 || val > UINT16_MAX)
		throw invalidPortException();
	return (static_cast<uint16_t>(val));
}
