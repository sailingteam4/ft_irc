#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <iostream>

class Server
{
    private:
        std::string port;
        std::string password;
    
    public:
        Server(std::string port, std::string password);
        ~Server();
};

#endif
