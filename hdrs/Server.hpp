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
#include <sstream>
#include "Channel.hpp"


class Server
{
    private:
		// Basic args
        uint16_t	port;
        std::string password;

		// Socket and client management
        int listening_socket;
        std::vector<int> client_sockets;
        std::map<int, std::string> client_nicknames;
        std::map<int, bool> client_authenticated;
        fd_set master_set;
        int fdmax;
        
        // Channel management
        std::vector<Channel> channels;

        // HandleClient.cpp
        void handleNewConnection();
        void handleClientData(int client_fd);
        void handleClientMessage(int client_fd, const std::string& message);
        bool authenticateClient(int client_fd, const std::string& pass);
        
        // IRC command handlers
        void handlePass(int client_fd, const std::string& message);
        void handleNick(int client_fd, const std::string& message);
        void handlePing(int client_fd, const std::string& message);
        void handleQuit(int client_fd, const std::string& message);
        void handleJoin(int client_fd, const std::string& message);
        void handleTopic(int client_fd, const std::string& message);
        void handlePrivmsg(int client_fd, const std::string& message);
        void handleMode(int client_fd, const std::string& message);
        std::vector<std::pair<char, char> > handleWhatMode(int client_fd, const std::string& mode);
        
        // Channel methods
        Channel* findChannel(const std::string& channel_name);
        void createChannel(const std::string& channel_name);
        void broadcastToChannel(const std::string& message, const std::string& channel_name, int exclude_fd = -1);
        
    public:
		// Constructor and Destructor
        Server(uint16_t port, std::string password);
        ~Server();

		// Server methods
        bool initialize();
        void run();
        void cleanupSocket(int socket_fd);

        // Utils
        long stoi(const char* s);
};

#endif
