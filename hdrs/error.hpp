/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 15:38:51 by mbico             #+#    #+#             */
/*   Updated: 2025/04/17 16:08:57 by mateo            ###   ########.fr       */
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
}

namespace errStup {
	class	socketCouldNotCreate: public std::exception
	{
		const char	*what() const throw();
	};
	class	socketFailedToSet: public std::exception
	{
		const char	*what() const throw();
	};
	class	socketCouldNotBind: public std::exception
	{
		const char	*what() const throw();
	};
	class	socketCouldNotListen: public std::exception
	{
		const char	*what() const throw();
	};
}

namespace	errEx {
	class	selectFailed: public std::exception
	{
		const char	*what() const throw();
	};
}
