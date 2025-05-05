/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:36:49 by mbico             #+#    #+#             */
/*   Updated: 2025/05/05 17:44:00 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

enum	errno_irc_code {
	NO_ERROR = 0,
	BAD_CONVERSION = 1,
};

extern	errno_irc_code	errno_irc;

namespace errPsg {
	class	invalidPortException: public std::exception
	{
		const char	*what() const throw();
	};
	class	invalidPasswordException: public std::exception
	{
		const char	*what() const throw();
	};
	class	invalidArgException: public std::exception
	{
		const char	*what() const throw();
	};
}
