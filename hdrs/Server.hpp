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
		// Basic args
        std::string port;
        std::string password;

		// Socket and client management
        int listening_socket;
        std::vector<int> client_sockets;
        std::map<int, std::string> client_nicknames;
        fd_set master_set;
        int fdmax;

        // HandleClient.cpp
        void handleNewConnection();
        void handleClientData(int client_fd);
        void handleClientMessage(int client_fd, const std::string& message);
        
    public:
		// Constructor and Destructor
        Server(std::string port, std::string password);
        ~Server();

		// Server methods
        bool initialize();
        void run();
        void cleanupSocket(int socket_fd);

        // Utils
        long stoi(const char* s);
};

#endif
