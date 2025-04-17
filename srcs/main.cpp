/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrontey <nrontey@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 20:43:18 by mbico             #+#    #+#             */
/*   Updated: 2025/04/17 16:35:08 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"

errno_irc_code errno_irc = NO_ERROR;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Error: Invalid number of arguments" << std::endl;
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    
    std::string port(argv[1]);
    std::string password(argv[2]);

    if (password.empty())
    {
        std::cerr << "Error: Password cannot be empty" << std::endl;
        return 1;
    }

	try {
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
