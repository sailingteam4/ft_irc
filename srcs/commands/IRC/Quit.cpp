#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"

void Server::handleQuit(int client_fd, const std::string& message)
{
    std::string quit_message = "Quitting";
    size_t colonPos = message.find(" :");
    if (colonPos != std::string::npos)
        quit_message = message.substr(colonPos + 2);
    
    std::string nickname = client_nicknames[client_fd];
    if (nickname.empty())
    {
        std::stringstream ss;
        ss << "user" << client_fd;
        nickname = ss.str();
    }
    
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->hasUser(client_fd))
        {
            std::string quitMsg = ":" + nickname + "!" SERVER_NAME " QUIT :" + quit_message + "\r\n";
            broadcastToChannel(quitMsg, it->getName(), client_fd);
        }
    }
    
    std::string response = "ERROR :Closing connection\r\n";
    send(client_fd, response.c_str(), response.size(), 0);
    
    std::cout << "Client " << client_fd << " (" << nickname << ") quit: " << quit_message << std::endl;
}
