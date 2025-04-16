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
        int listening_socket;
        std::vector<int> client_sockets;
        std::map<int, std::string> client_nicknames;
        fd_set master_set;
        int fdmax;
        
    public:
        Server(std::string port, std::string password);
        ~Server();
        
        bool initialize();
        void run();
        void handleNewConnection();
        void handleClientData(int client_fd);
        void handleClientMessage(int client_fd, const std::string& message);
        void cleanupSocket(int socket_fd);
        
        // Utility methods
        long stoi(const char* s);
};

#endif
