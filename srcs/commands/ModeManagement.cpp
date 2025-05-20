#include "Server.hpp"
#include "error.hpp"
#include "ft_irc.hpp"

static std::string targetDecoupe(const std::string& target, size_t index);




/*
Mode | Name | Add (+) | Suppr (-) | Argument needed ?
i | Invite-only | Activate “sur invit” | Deactivate option | ❌ Never argument
t | Topic restricted to op only | Activate restriction | Deactivate | ❌ Never an argument
k | Channel key (password) | Set key | Delete key | ✅ Add: yes❌ Delete: no
o | Channel operator | Set status | Remove status | ✅ Add/delete: yes
l | Limit users | Sets a limit | Removes the limit | ✅ Add: yes❌ Remove: no
*/

// /MODE #Channel +o operator
// params = #Channel +o operator
// channelName = #Channel 
// mode = +o 
// arg = operator

void Server::handleMode(int client_fd, const std::string& message) {
	
    if (client_info.find(client_fd) == client_info.end() || 
        !client_info[client_fd].hasNick || !client_info[client_fd].hasUser)
    {
        std::string errorMsg = ERR_NOTREGISTERED(client_nicknames[client_fd]);
        send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
	size_t startPos = message.find("MODE") + 5;
	std::string params = message.substr(startPos);
	
	params.erase(0, params.find_first_not_of(" "));
	
	size_t firstSpace = params.find(' ');
	std::string channelName;
	if (firstSpace != std::string::npos)
	{
		channelName = params.substr(0, firstSpace);
		params = params.substr(firstSpace + 1);
		std::cout << "channelName " << channelName << std::endl;
	}
	else
	{
		channelName = params;
		params = "";
	}
	
	size_t secondSpace = params.find(' ');
	std::string mode;
	std::string target;
	if (secondSpace != std::string::npos)
	{
		mode = params.substr(0, secondSpace);
		target = params.substr(secondSpace + 1);
	}
	else
	{
		mode = params;
		target = "";
	}

	std::vector<std::pair<char, char> > mode_map = handleWhatMode(client_fd, mode, channelName);
	std::vector<std::pair<char, std::pair<char, std::string> > > target_mode_map;
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

				if (modeLetter == 'i')
					ModeInvitation(modeLetter, sign, channelName, client_fd);

				if (modeLetter == 't')
					ModeTopic(modeLetter, sign, channelName, client_fd);

				if (modeLetter == 'k')
					ModeKey(modeLetter, sign, target_value, channelName, client_fd);

				if (modeLetter == 'o')
					ModeOperator(modeLetter, sign, target_value, channelName, client_fd);

				if (modeLetter == 'l')
					ModeLimit(modeLetter, sign, target_value, channelName, client_fd);
			}
		}
	}
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
		{			if (c == 'i' || c == 't' || c == 'k' || c == 'o' || c == 'l' )
			{				// Emit a warning for duplicate mode characters, using 472 (unknown mode)
				std::string errorMsg = ERR_UNKNOWNMODE(client_nicknames[client_fd], channelName, std::string(1, c));
				send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
				return (0);
			}			if (c != 'i' && c != 't' && c != 'k' && c != 'o' && c != 'l')
			{				// Send ERR_UNKNOWNMODE (472) for unknown mode characters
				std::string errorMsg = ERR_UNKNOWNMODE(client_nicknames[client_fd], channelName, std::string(1, c));
				send(client_fd, errorMsg.c_str(), errorMsg.size(), 0);
				return (1);
			}
		}
	}
	return (1);
}


std::vector<std::pair<char, char> > Server::handleWhatMode(int client_fd, const std::string& mode, std::string channelName)
{
	std::vector<std::pair<char, char> > mode_map;
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