#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"

void Server::handlePing(int client_fd, const std::string& message)
{
    std::string pingParam = message.substr(5);
    std::string response = "PONG :" + pingParam + "\r\n";
    send(client_fd, response.c_str(), response.size(), 0);
}