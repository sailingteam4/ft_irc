#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"

Server::Server(uint16_t port, std::string password) : port(port), password(password), listening_socket(-1), fdmax(0)
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

bool Server::initialize()
{
    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_socket == -1)
		throw errStup::socketCouldNotCreate(); 

    int yes = 1;
    if (setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		throw errStup::socketFailedToSet();

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening_socket, (sockaddr*)&hint, sizeof(hint)) == -1)
		throw errStup::socketCouldNotBind();

    if (listen(listening_socket, SOMAXCONN) == -1)
		throw errStup::socketCouldNotListen();

    FD_SET(listening_socket, &master_set);
    fdmax = listening_socket;

    std::cout << "Server is running on port " << port << std::endl;
    std::cout << "Waiting for connections..." << std::endl;

    return true;
}

void Server::run()
{
    fd_set read_fds;
    
    while (!stopServer)
    {
        read_fds = master_set;
        
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
			throw errEx::selectFailed();
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
    
    
    std::vector<size_t> emptyChannelsIndexes;
    for (size_t i = 0; i < channels.size(); ++i)
    {
        channels[i].removeUser(socket_fd);
        
        if (channels[i].getUsers().empty())
        {
            std::cout << "Marking empty channel for removal: " << channels[i].getName() << std::endl;
            emptyChannelsIndexes.push_back(i);
        }
    }
    
    for (std::vector<size_t>::reverse_iterator rit = emptyChannelsIndexes.rbegin(); 
         rit != emptyChannelsIndexes.rend(); ++rit)
    {
        std::cout << "Removing empty channel: " << channels[*rit].getName() << std::endl;
        channels.erase(channels.begin() + *rit);
    }
    
    client_nicknames.erase(socket_fd);
    client_authenticated.erase(socket_fd);
    client_info.erase(socket_fd);
    client_buffer.erase(socket_fd);
}
