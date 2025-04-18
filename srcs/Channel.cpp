#include "Channel.hpp"
#include <algorithm>
#include <sstream>

Channel::Channel(const std::string& channelName) : name(channelName), topic("")
{
    if (name[0] != '#')
        name = "#" + name;
}

Channel::~Channel() {}

const std::string& Channel::getName() const
{
    return name;
}

const std::string& Channel::getTopic() const
{
    return topic;
}

const std::vector<int>& Channel::getUsers() const
{
    return users;
}

bool Channel::isOperator(int user_fd) const
{
    std::map<int, bool>::const_iterator it = operators.find(user_fd);
    return (it != operators.end() && it->second);
}

void Channel::setTopic(const std::string& newTopic)
{
    topic = newTopic;
}

bool Channel::addUser(int user_fd)
{
    if (hasUser(user_fd))
        return false;
        
    users.push_back(user_fd);

    if (users.size() == 1)
        operators[user_fd] = true;
    else
        operators[user_fd] = false;
        
    return true;
}

bool Channel::removeUser(int user_fd)
{
    std::vector<int>::iterator it = std::find(users.begin(), users.end(), user_fd);
    if (it != users.end())
    {
        users.erase(it);
        operators.erase(user_fd);
        return true;
    }
    return false;
}

void Channel::setOperator(int user_fd, bool op_status)
{
    operators[user_fd] = op_status;
}

bool Channel::hasUser(int user_fd) const
{
    return std::find(users.begin(), users.end(), user_fd) != users.end();
}

std::string Channel::getUserList(const std::map<int, std::string>& nicknames) const
{
    std::string userList;
    
    for (std::vector<int>::const_iterator it = users.begin(); it != users.end(); ++it)
    {
        if (it != users.begin())
            userList += " ";

        if (isOperator(*it))
            userList += "@";

        std::map<int, std::string>::const_iterator nick_it = nicknames.find(*it);
        if (nick_it != nicknames.end() && !nick_it->second.empty())
            userList += nick_it->second;
        else
        {
            std::stringstream ss;
            ss << "user" << *it;
            userList += ss.str();
        }
    }
    
    return userList;
}