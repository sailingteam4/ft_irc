/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 15:38:51 by mbico             #+#    #+#             */
/*   Updated: 2025/04/14 18:43:16 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.hpp"

const char	*invalidPortException::what() const throw()
{
	return("invalidPortException");	
}
