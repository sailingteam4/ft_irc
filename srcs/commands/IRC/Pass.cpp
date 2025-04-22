#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"

void Server::handlePass(int client_fd, const std::string& message)
{
    size_t pos = message.find("PASS") + 5;
    std::string pass = message.substr(pos);
    if (pass.find("\r") != std::string::npos)
        pass.erase(pass.find("\r"));
    else if (pass.find("\n") != std::string::npos)
        pass.erase(pass.find("\n"));
    
    if (authenticateClient(client_fd, pass))
    {
        std::string response = "Authentication successful\r\n";
        send(client_fd, response.c_str(), response.size(), 0);
    }
    else
    {
        std::string response = "Authentication failed: invalid password\r\n";
        send(client_fd, response.c_str(), response.size(), 0);
    }
}