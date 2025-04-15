/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 15:38:51 by mbico             #+#    #+#             */
/*   Updated: 2025/04/15 21:04:51 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.hpp"
	
const char	*errEx::invalidPortException::what() const throw() {
	return("invalidPortException");	
}
