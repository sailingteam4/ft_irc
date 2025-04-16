#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <fcntl.h>
#include <vector>
#include <map>

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
