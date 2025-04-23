#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"

static std::string targetDecoupe(const std::string& target, size_t index);




/*
Mode | Nom | Ajout (+) | Suppression (-) | Argument requis ?
i | Invite-only | Active "sur invit" | Désactive l'option | ❌ Jamais d'argument
t | Topic restricted to op only | Active restriction | Désactive | ❌ Jamais d'argument
k | Channel key (mot de passe) | Définit une clé | Supprime la clé | ✅ Ajout: oui❌ Supp: non
o | Channel operator | Donne le statut | Le retire | ✅ Ajout/suppression : oui
l | Limit users | Définit une limite | Supprime la limite | ✅ Ajout: oui❌ Supp: non
*/

void Server::handleMode(int client_fd, const std::string& message) {
	
	// J'ai rajouté un check pour savoir si le client est bien authentifié avant de faire quoi que ce soit
    if (client_info.find(client_fd) == client_info.end() || 
        !client_info[client_fd].hasNick || !client_info[client_fd].hasUser)
    {
        std::string errorMsg = ":" SERVER_NAME " 451 :You have not registered\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
	// Exemple de message : "/MODE #Channel +o operator"
	// le code va tout decomposer le message pour en extraire les informations

	// /MODE #Channel +o operator
	// /MODE #Channel +ok operator 50
	// /MODE #Channel +o+k operator 50


	// /MODE #Channel +k -k operator


	// params = #Channel +o operator
	// channelName = #Channel 
	// mode = +o 
	// arg = operator
	//	mettre les/le mode(s) dans un dico avec son signe
	//	et refaire un dico en mettant pour chaque cle ( le mode) sa valeur (l'arg). sachant que tout ne prend pas des args
	//
	//


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

	std::vector<std::pair<char, char> > mode_map = handleWhatMode(client_fd, mode, channelName);
	std::vector<std::pair<char, std::pair<char, std::string> > > target_mode_map; // Changed to store mode letter, sign, and target
	if (mode_map.empty())
	{
		//en gros quand parsmode ressors l'erreur sans code derreur elle arrive ici, (s'active aussi quand on /join car join appel mode ????)
		std::cout << "ERROR IN std::map" << std::endl;
	}
	if (!mode_map.empty())
	{
		size_t argIndex = 0;
		for (std::vector<std::pair<char, char> >::iterator it = mode_map.begin(); it != mode_map.end(); ++it)
		{
			char modeLetter = it->first;
			char sign = it->second;
			std::string value;
		
			if ((modeLetter == 'k' && sign == '+') || (modeLetter == 'o') || (modeLetter == 'l' && sign == '+'))
			{
				value = targetDecoupe(target, argIndex++);
			}
			else
			{
				value = "";
			}
		
			target_mode_map.push_back(std::make_pair(modeLetter, std::make_pair(sign, value)));
		}
		if (!target_mode_map.empty())
		{
			for (std::vector<std::pair<char, std::pair<char, std::string> > >::iterator it = target_mode_map.begin(); it != target_mode_map.end(); ++it)
			{
				char modeLetter = it->first;
				char sign = it->second.first;
				std::string target_value = it->second.second;

				//fonction qui gere i
				if (modeLetter == 'i')
				{}
					// std::cout << "appel de 1" << modeLetter << " sign:" << sign << " mot: " <<target_value << std::endl;

				//fonction qui gere t
				if (modeLetter == 't')
				{
					ModeTopic(modeLetter, sign, channelName, client_fd);
				}

				//fonction qui gere k
				if (modeLetter == 'k')
				{}
					// std::cout << "appel de 3" << modeLetter << " sign:" << sign << " mot: " <<target_value << std::endl;

				//fonction qui gere o
				if (modeLetter == 'o')
				{
					// std::cout << "appel de 4" << modeLetter << " sign:" << sign << " mot: " <<target_value << std::endl;
					ModeOperator(modeLetter, sign, target_value, channelName, client_fd);
				}

				//fonction qui gere l
				if (modeLetter == 'l')
				{
					ModeLimit(modeLetter, sign, target_value, channelName, client_fd);
				}
				
				(void) target_value;
			}
		}
	}
		// TEST FONCTION
		// for (std::vector<std::pair<char, std::string> >::const_iterator it = target_mode_map.begin(); it != target_mode_map.end(); ++it)
		// {
		// 	std::cout << "Mode: " << it->first << ", Argument: " << (it->second.empty() ? "(none)" : it->second) << std::endl;
		// }
}
	

static std::string targetDecoupe(const std::string& target, size_t index)
{
    std::istringstream iss(target);
    std::string word;
    size_t currentIndex = 0;
    
    while (iss >> word)
	{
        if (currentIndex == index)
		{
            return (word);
        }
        currentIndex++;
    }
    
    return ("");
}

int Server::parsmode(int client_fd, const std::string& mode, std::string channelName)
{
	typedef char char_mode;
	typedef int nbr_mode;
	std::map<char_mode, nbr_mode> pars_map;

	for (std::string::size_type i = 0; i < mode.length(); ++i)
	{
		char c = mode[i];

		if (c == '+' || c == '-')
			continue;
		++pars_map[c];

		if ((pars_map[c] > 1 && (c == 'i' || c == 't' || c == 'k' || c == 'o' || c == 'l')) || (c != 'i' && c != 't' && c != 'k' && c != 'o' && c != 'l'))
		{
			if (c == 'i' || c == 't' || c == 'k' || c == 'o' || c == 'l' )
			{
				//ATTENTION NE PAS METTRE DE CODE D'ERREUR ICI, ON EST DANS LE CAS OU LA COMMANDE EST PAR EXEMPLE +kk DONC COMME IRC DE BASE NA PAS DE CODE DERREUR POUR CA ON EN MET PAS
				// std::cout << "Erreur mode 1: " << mode << std::endl;
				return (0);
			}
			if (c != 'i' && c != 't' && c != 'k' && c != 'o' && c != 'l')
			{
				//METTRE UN CODE DERREUR ICI NORMALEMENT LE 472, C'EST ERROR UNKNOW MODE DANS LA NORME
				// std::cout << "Erreur mode 2: " << mode << std::endl;
				std::string errorMsg = ":" SERVER_NAME " 472 " + client_nicknames[client_fd] + " " + channelName + " :ERROR UNKNOWN MODE\r\n";
				send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
				return (1);
			}
		}
	}
	return (1);
}


// en gros je suis obliger d'utiliser cette horreur std::vector<std::pair<char, char> > pour laisser le dico non trier sinon par exemple +kl-o-i devenai 'i' < 'k' < 'l' < 'o' a cause de l'odre alphabetique. On l'emmerde l'ordre alphabetique pas vrai la team ?!
std::vector<std::pair<char, char> > Server::handleWhatMode(int client_fd, const std::string& mode, std::string channelName)
{
	std::vector<std::pair<char, char> > mode_map;
	//mode, signe + ou -
	if (parsmode(client_fd, mode, channelName) == 0)
		return mode_map;

	char currentSign = '+';
	for (std::string::size_type i = 0; i < mode.length(); ++i)
	{
		char c = mode[i];
		if (c == '+' || c == '-')
		{
			currentSign = c;
		}
		else if (c == 'i' || c == 't' || c == 'k' || c == 'o' || c == 'l')
		{
			mode_map.push_back(std::make_pair(c, currentSign));
		}
	}

	return mode_map;
}