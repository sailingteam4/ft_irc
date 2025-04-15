/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 15:38:51 by mbico             #+#    #+#             */
/*   Updated: 2025/04/15 21:01:55 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

namespace errorException {
	class	invalidPortException: public std::exception
	{
		const char	*what() const throw();
	};
}
