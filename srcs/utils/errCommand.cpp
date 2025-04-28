/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 04:53:28 by mateo             #+#    #+#             */
/*   Updated: 2025/04/28 07:04:20 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.hpp"
#include <sys/socket.h>

bool	errCommand(bool condition, int client_fd, const std::string& err)
{
	if (condition)
	{
		send(client_fd, err.c_str(), err.size(), 0);
		return (true);
	}
	return (false);
}
