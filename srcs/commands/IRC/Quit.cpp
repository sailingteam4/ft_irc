#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"

void Server::handleQuit(int client_fd, const std::string& message)
{
    (void)message;
    std::string response = "Bye bye !\r\n";
    send(client_fd, response.c_str(), response.size(), 0);
    cleanupSocket(client_fd);
}