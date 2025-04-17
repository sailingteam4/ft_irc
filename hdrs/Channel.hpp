#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>

class Channel
{
    private:
        std::string name;
        std::string topic;
        std::vector<int> users;
        std::map<int, bool> operators;

    public:
        // Constructor and Destructor
        Channel(const std::string& channelName);
        ~Channel();

        // Getters
        const std::string& getName() const;
        const std::string& getTopic() const;
        const std::vector<int>& getUsers() const;
        bool isOperator(int user_fd) const;
        
        // Setters
        void setTopic(const std::string& newTopic);
        
        // Channel management
        bool addUser(int user_fd);
        bool removeUser(int user_fd);
        void setOperator(int user_fd, bool op_status);
        bool hasUser(int user_fd) const;
        
        // User list as a formatted string for IRC responses
        std::string getUserList(const std::map<int, std::string>& nicknames) const;
};

#endif