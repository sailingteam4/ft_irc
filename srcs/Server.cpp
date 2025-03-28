#include "Server.hpp"

Server::Server(std::string port, std::string password) : port(port), password(password)
{
    std::cout << "Server created on port :  " << port << std::endl;
}

Server::~Server()
{
    std::cout << "Server shutdown" << std::endl;
}
