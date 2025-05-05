/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:56:43 by mbico             #+#    #+#             */
/*   Updated: 2025/05/05 19:36:27 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.hpp"
#include "bot.hpp"

errno_irc_code errno_irc = NO_ERROR;

int serverInit(uint16_t	port, std::string password) {
    int sockfd;
    struct hostent* host;
    struct sockaddr_in server_addr;

    host = gethostbyname(SERVER);
    if (!host) {
        std::cerr << "Erreur: impossible de résoudre l'hôte." << std::endl;
        return -1;
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Erreur: impossible de créer le socket." << std::endl;
        return -1;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    server_addr.sin_addr = *((struct in_addr*)host->h_addr);
    memset(&(server_addr.sin_zero), 0, 8);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0) {
        std::cerr << "Erreur: connexion échouée." << std::endl;
        return -1;
    }
    std::string login = "PASS " + password + "\r\nNICK " + "bot" + "\r\nUSER " + "bot" + NAME "\r\n";
    send(sockfd, login.c_str(), login.length(), 0);
	return (sockfd);
}

void	serverRun(int sockfd)
{

    while (true) {
		if (commandHandle(sockfd))
			break;
    }
    close(sockfd);
}

int	main(int argc, char *argv[])
{
	uint16_t	port;
	std::string	password;
	int16_t		sockfd;

	try {
		argValid(argc);
		port = portValid(argv[1]);
		password = passwordValid(argv[2]);
		sockfd = serverInit(port, password);
		if (sockfd < 0)
			return (1);
		serverRun(sockfd);
	}
	catch (std::exception& e)
	{
		std::cerr << RED << e.what() << WHITE << std::endl;
	}
}


