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

		// Channel modes
		bool invite_only;
		bool topic_protection;
		std::string key;
		unsigned int user_limit;
		
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

		// Channel modes
		bool isInviteOnly() const;
		bool isTopicProtected() const;
		bool hasKey() const;
		bool hasUserLimit() const;

		void setInviteOnly(bool status);
		void setTopicProtected(bool status);
		void setKey(const std::string& newKey);
		void setUserLimit(unsigned int limit);
		
		// User list as a formatted string for IRC responses
		std::string getUserList(const std::map<int, std::string>& nicknames) const;
};

#endif