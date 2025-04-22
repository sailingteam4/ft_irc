#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"

void Server::handleNick(int client_fd, const std::string& message)
{
    std::string nickname = message.substr(5);
    if (nickname.find("\r") != std::string::npos)
        nickname.erase(nickname.find("\r"));
    else if (nickname.find("\n") != std::string::npos)
        nickname.erase(nickname.find("\n"));
    
    for (std::map<int, std::string>::const_iterator it = client_nicknames.begin(); it != client_nicknames.end(); ++it)
    {
        if (it->first != client_fd && it->second == nickname)
        {
            std::string errorMsg = ":" SERVER_NAME " 433 * " + nickname + " :Nickname is already in use\r\n";
            send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
            return;
        }
    }
    
    client_nicknames[client_fd] = nickname;
    
    if (client_info.find(client_fd) == client_info.end())
        client_info[client_fd] = UserInfo();
    
    client_info[client_fd].hasNick = true;

    std::string response = "NICK command received. Your nickname is now " + nickname + "\r\n";
    send(client_fd, response.c_str(), response.size(), 0);

    std::cout << "Client socket " << client_fd << " set nickname to: " << nickname << std::endl;
    
    checkRegistrationStatus(client_fd);
}