#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"



void Server::handleMode(int client_fd, const std::string& message) {
	
	// Exemple de message : "/MODE #Channel +o operator"
	// le code va tout deconposer le message pour en extraire les informations

	// /MODE #Channel +o operator
	// params = #Channel +o operator
	// channelName = #Channel 
	// mode = +o 
	// arg = operator


    size_t startPos = message.find("MODE") + 5;
    std::string params = message.substr(startPos);
    
    // On supprime les espaces au début, jsp si on a trop le droit mais bref sinon nsm
    params.erase(0, params.find_first_not_of(" "));
    
	// sors le nom du channel
    size_t firstSpace = params.find(' ');
    std::string channelName;
    if (firstSpace != std::string::npos)
	{
        channelName = params.substr(0, firstSpace);
        params = params.substr(firstSpace + 1);
    }
	else
	{
        channelName = params;
        params = "";
    }
    
    // sors le mode ou les modes
    size_t secondSpace = params.find(' ');
	std::string mode;
	std::string target;
    if (secondSpace != std::string::npos)
	{
        mode = params.substr(0, secondSpace);
        // sors l'arg
        target = params.substr(secondSpace + 1);
    }
	else
	{
        mode = params;
        target = "";
    }
    
	(void)client_fd; // pas used mais pour parsing apres utils

	// potit test debug 
	// std::cout << "Canal: " << channelName << std::endl;
    // std::cout << "Mode: " << mode << std::endl;
    // std::cout << "Opérateur: " << target << std::endl;
}