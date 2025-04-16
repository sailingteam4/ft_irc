#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"

Server::Server(std::string port, std::string password) : port(port), password(password), listening_socket(-1), fdmax(0)
{
    std::cout << "Server created on port: " << port << std::endl;
    FD_ZERO(&master_set);
}

Server::~Server()
{
    for (std::vector<int>::iterator it = client_sockets.begin(); it != client_sockets.end(); ++it)
    {
        close(*it);
    }
    
    if (listening_socket != -1)
        close(listening_socket);
        
    std::cout << "Server shutdown" << std::endl;
}

long Server::stoi(const char *s)
{
    long i;
    i = 0;
    while(*s >= '0' && *s <= '9')
    {
        i = i * 10 + (*s - '0');
        s++;
    }
    return i;
}

bool Server::initialize()
{
    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_socket == -1)
    {
        std::cerr << "Error: Could not create socket" << std::endl;
        return false;
    }

    int yes = 1;
    if (setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        std::cerr << "Error: Failed to set socket options" << std::endl;
        return false;
    }

    int nb_port = stoi(port.c_str());

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(nb_port);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening_socket, (sockaddr*)&hint, sizeof(hint)) == -1)
    {
        std::cerr << "Error: Could not bind socket" << std::endl;
        return false;
    }

    if (listen(listening_socket, SOMAXCONN) == -1)
    {
        std::cerr << "Error: Could not listen on socket" << std::endl;
        return false;
    }

    FD_SET(listening_socket, &master_set);
    fdmax = listening_socket;

    std::cout << "Server is running on port " << port << std::endl;
    std::cout << "Waiting for connections..." << std::endl;

    return true;
}

void Server::run()
{
    fd_set read_fds;
    
    while (true)
    {
        read_fds = master_set;
        
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
        {
            std::cerr << "Error: select() failed" << std::endl;
            break;
        }
        
        for (int i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds))
            {
                if (i == listening_socket)
                {
                    handleNewConnection();
                }
                else
                {
                    handleClientData(i);
                }
            }
        }
    }
}

void Server::handleNewConnection()
{
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    
    int newfd = accept(listening_socket, (sockaddr*)&client, &clientSize);
    if (newfd == -1)
    {
        std::cerr << "Error: Could not accept connection" << std::endl;
        return;
    }
    
    FD_SET(newfd, &master_set);
    if (newfd > fdmax)
    {
        fdmax = newfd;
    }
    
    client_sockets.push_back(newfd);
    
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);
    
    int result = getnameinfo((sockaddr*)&client, sizeof(client), 
                           host, NI_MAXHOST, 
                           service, NI_MAXSERV, 0);
    if (result == 0)
    {
        std::cout << host << " connected on " << service << std::endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
    }
    
    std::cout << "New connection: socket fd=" << newfd 
             << ", ip=" << host << std::endl;

    std::string welcomeMessage = "SALUT GROS BG !\n";
    send(newfd, welcomeMessage.c_str(), welcomeMessage.size(), 0);
}

void Server::handleClientData(int client_fd)
{
    char buf[4096];
    memset(buf, 0, 4096);
    
    int bytesReceived = recv(client_fd, buf, 4096, 0);
    if (bytesReceived <= 0)
    {
        if (bytesReceived == 0)
        {
            std::cout << "Client socket " << client_fd << " disconnected" << std::endl;
        }
        else
        {
            std::cerr << "Error: Could not receive data from client socket " << client_fd << std::endl;
        }
        cleanupSocket(client_fd);
    }
    else
    {
        std::string message(buf, 0, bytesReceived);
        handleClientMessage(client_fd, message);
    }
}

void Server::handleClientMessage(int client_fd, const std::string& message)
{
    std::cout << "Received from socket " << client_fd << ": " << message << std::endl;

    if (message.find("PING") != std::string::npos)
    {
        std::string response = "PONG :" + message.substr(5);
        send(client_fd, response.c_str(), response.size(), 0);
    }
    else if (message.find("QUIT") != std::string::npos)
    {
        std::string response = "Bye bye !\n";
        send(client_fd, response.c_str(), response.size(), 0);
        cleanupSocket(client_fd);
    }
    else if (message.find("NICK") != std::string::npos)
    {
        std::string nickname = message.substr(5);
        nickname.erase(nickname.find("\r"));
        client_nicknames[client_fd] = nickname;

        std::string response = "NICK command received. Your nickname is now " + nickname + "\n";
        send(client_fd, response.c_str(), response.size(), 0);

        std::cout << "Client socket " << client_fd << " set nickname to: " << nickname << std::endl;
    }
    else if (message.find("JOIN") != std::string::npos)
    {
        size_t pos = message.find("JOIN") + 5;
        std::string channel = message.substr(pos);
        channel.erase(channel.find("\r"));

        std::string nickname = client_nicknames[client_fd];

        std::string joinMessage = ":" + nickname + "!user@localhost JOIN :" + channel + "\r\n";
        send(client_fd, joinMessage.c_str(), joinMessage.size(), 0);

        std::string response = ":irc.example.com 331 " + nickname + " " + channel + " :No topic is set\r\n";
        send(client_fd, response.c_str(), response.size(), 0);

        response = ":irc.example.com 353 " + nickname + " = " + channel + " :@" + nickname + "\r\n";
        send(client_fd, response.c_str(), response.size(), 0);

        response = ":irc.example.com 366 " + nickname + " " + channel + " :End of /NAMES list.\r\n";
        send(client_fd, response.c_str(), response.size(), 0);

        std::cout << "Client " << nickname << " joined channel: " << channel << std::endl;
    }
    else if (message.find("TOPIC") != std::string::npos)
    {
        size_t pos = message.find("TOPIC") + 6;
        std::string channel = message.substr(pos);
        channel.erase(channel.find("\r"));
        std::string nickname = client_nicknames[client_fd];

        std::string topicMessage = ":" + nickname + "!user@localhost TOPIC " + channel + " :New topic set\r\n";
        send(client_fd, topicMessage.c_str(), topicMessage.size(), 0);

        std::cout << "Client " << nickname << " set topic for channel: " << channel << std::endl;
    }
    else
    {
        std::string response = "421 Unknown command\r\n";
        send(client_fd, response.c_str(), response.size(), 0);
    }
}

void Server::cleanupSocket(int socket_fd)
{
    close(socket_fd);
    FD_CLR(socket_fd, &master_set);
    
    for (std::vector<int>::iterator it = client_sockets.begin(); it != client_sockets.end(); ++it)
    {
        if (*it == socket_fd)
        {
            client_sockets.erase(it);
            break;
        }
    }
    
    client_nicknames.erase(socket_fd);
}
