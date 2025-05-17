#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"
#include <sstream>

void Server::handleUser(int client_fd, const std::string& message)
{
    if (client_info.find(client_fd) != client_info.end() && client_info[client_fd].hasUser) {
        std::string nickname = client_nicknames[client_fd];
        std::string error = ":" SERVER_NAME " 462 " + nickname + " :You may not reregister\r\n";
        send(client_fd, error.c_str(), error.size(), 0);
        return;
    }

    std::string params = message.substr(5);
    
    if (params.find("\r") != std::string::npos)
        params.erase(params.find("\r"));
    else if (params.find("\n") != std::string::npos)
        params.erase(params.find("\n"));
    
    size_t colonPos = params.find(" :");
    std::string realname;
    
    if (colonPos == std::string::npos) {
        std::string nickname = client_nicknames.count(client_fd) ? client_nicknames[client_fd] : "*";
        std::string error = ":" SERVER_NAME " 461 " + nickname + " USER :Not enough parameters\r\n";
        send(client_fd, error.c_str(), error.size(), 0);
        return;
    }
    
    realname = params.substr(colonPos + 2);
    params = params.substr(0, colonPos);
    
    std::istringstream iss(params);
    std::string username, hostname, servername;
    
    if (!(iss >> username >> hostname >> servername)) {
        std::string nickname = client_nicknames.count(client_fd) ? client_nicknames[client_fd] : "*";
        std::string error = ":" SERVER_NAME " 461 " + nickname + " USER :Not enough parameters\r\n";
        send(client_fd, error.c_str(), error.size(), 0);
        return;
    }
    
    if (username.empty() || username.find(' ') != std::string::npos) {
        std::string nickname = client_nicknames.count(client_fd) ? client_nicknames[client_fd] : "*";
        std::string error = ":" SERVER_NAME " 432 " + nickname + " :Invalid username\r\n";
        send(client_fd, error.c_str(), error.size(), 0);
        return;
    }
    
    if (client_info.find(client_fd) == client_info.end())
        client_info[client_fd] = UserInfo();
    
    UserInfo& info = client_info[client_fd];
    info.username = username;
    info.hostname = hostname;
    info.servername = servername;
    info.realname = realname;
    info.hasUser = true;
    
    std::cout << "Client socket " << client_fd << " registered user: " << username << " realname: " << realname << std::endl;
    
    checkRegistrationStatus(client_fd);
}

void Server::checkRegistrationStatus(int client_fd)
{
    if (client_info.find(client_fd) != client_info.end() && 
        client_info[client_fd].hasNick && client_info[client_fd].hasUser &&
        !client_info[client_fd].welcomeSent)
    {
        std::string nickname = client_nicknames[client_fd];
        std::string username = client_info[client_fd].username;
        std::string realname = client_info[client_fd].realname;
        
        std::string numeric001 = ":" SERVER_NAME " 001 " + nickname + " :Bienvenue gros bg, " + 
                                nickname + "!" + username + "@" SERVER_NAME + "\r\n";
        send(client_fd, numeric001.c_str(), numeric001.size(), 0);
        
        std::string numeric002 = ":" SERVER_NAME " 002 " + nickname + " :Feurquoicoupbebou " SERVER_NAME ", version 1.0\r\n";
        send(client_fd, numeric002.c_str(), numeric002.size(), 0);
        
        std::string numeric003 = ":" SERVER_NAME " 003 " + nickname + " :This server was created on April 21, 2025\r\n";
        send(client_fd, numeric003.c_str(), numeric003.size(), 0);
        
        std::string numeric004 = ":" SERVER_NAME " 004 " + nickname + " :" SERVER_NAME " 1.0 o o\r\n";
        send(client_fd, numeric004.c_str(), numeric004.size(), 0);
        
        std::cout << "Client " << nickname << " (" << username << ") has completed registration" << std::endl;
        
        client_info[client_fd].welcomeSent = true;
    }
}