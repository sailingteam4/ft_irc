/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrontey <nrontey@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 20:43:18 by mbico             #+#    #+#             */
/*   Updated: 2025/04/18 01:16:34 by nrontey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"

errno_irc_code errno_irc = NO_ERROR;

int main(int argc, char *argv[])
{
    uint16_t	port;
	std::string	password;

	try {
		argValid(argc);
		port = portValid(argv[1]);
		password = passwordValid(argv[2]);
		Server server(port, password);
		
		if (!server.initialize())
			return 1;

		server.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << RED << e.what() << WHITE << std::endl;
	}
    return 0;
}
