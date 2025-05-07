/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 23:59:18 by mateo             #+#    #+#             */
/*   Updated: 2025/05/08 00:19:05 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <ctime>
#include <string>
#include <stdint.h>

int	randint(int min, int max) 
{
	return (min + rand() % (max - min + 1));
}

uint32_t	countSymbole(std::string str)
{
	uint32_t count = 0;

	for (int i = 0; i < (int)str.length(); i ++)
	{
		if (static_cast<unsigned char>(str[i]) < 20 || static_cast<unsigned char>(str[i]) > 126)
			count ++;
	}
	return (count);

}
