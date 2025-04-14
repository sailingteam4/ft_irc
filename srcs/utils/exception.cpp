/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 15:38:51 by mbico             #+#    #+#             */
/*   Updated: 2025/03/31 14:50:08 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exception.hpp"

const char	*invalidPortException::what() const throw()
{
	return("invalidPortException");	
}
