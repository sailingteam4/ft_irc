/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 15:38:51 by mbico             #+#    #+#             */
/*   Updated: 2025/04/15 21:04:28 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

enum	errno_irc_code {
	NO_ERROR = 0,
	BAD_CONVERSION = 1,
};

extern	errno_irc_code	errno_irc;

namespace errEx {
	class	invalidPortException: public std::exception {
		const char	*what() const throw();
	};
}
