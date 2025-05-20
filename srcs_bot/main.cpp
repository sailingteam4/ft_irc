/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:56:43 by mbico             #+#    #+#             */
/*   Updated: 2025/05/20 18:11:35 by mbico            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.hpp"
#include "bot.hpp"
#include "Player.hpp"
#include "Table.hpp"
#include "Screen.hpp"
#include <csignal>
#include <sys/select.h>

errno_irc_code errno_irc = NO_ERROR;
bool stopServer = false;

void signalHandler(int signal) {
    if (signal == SIGINT){
		std::cout << std::endl;
		stopServer = true;
	}
}

int serverInit(uint16_t	port, std::string password) {
    int sockfd;
    struct hostent* host;
    struct sockaddr_in server_addr;

    host = gethostbyname(SERVER);
    if (!host) {
        std::cerr << "Error: bad host" << std::endl;
        return -1;
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error: sockfd error" << std::endl;
        return -1;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    server_addr.sin_addr = *((struct in_addr*)host->h_addr);
    memset(&(server_addr.sin_zero), 0, 8);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0) {
        std::cerr << "Error: can't connect" << std::endl;
		close(sockfd);
        return -1;
    }
    std::string login = "PASS " + password + "\r\nNICK " + "bot" + "\r\nUSER bot 0 * :" + "bot" + NAME "\r\n";
    send(sockfd, login.c_str(), login.length(), 0);
	return (sockfd);
}

void	serverRun(int sockfd, fd_set master)
{
    while (!stopServer) {
		if (commandHandle(sockfd, master))
			break;
    }
	FD_CLR(sockfd, &master);
    close(sockfd);
}

int	main(int argc, char *argv[])
{
	uint16_t	port;
	std::string	password;
	int16_t		sockfd;
	fd_set		master;

	std::signal(SIGINT, signalHandler);
	try {
		argValid(argc);
		port = portValid(argv[1]);
		password = passwordValid(argv[2]);
		sockfd = serverInit(port, password);
		FD_ZERO(&master);
		FD_SET(sockfd, &master);
		if (sockfd < 0)
			return (1);
		serverRun(sockfd, master);
	}
	catch (std::exception& e)
	{
		std::cerr << RED << e.what() << WHITE << std::endl;
	}
}


