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
		std::vector<int> invited_users;

		// Channel modes
		bool invite_only;
		bool topic_protection;
		std::string key;
		unsigned long user_limit;
		
		public:
		// Constructor and Destructor
		Channel(const std::string& channelName);
		~Channel();
		
		// Getters
		const std::string& getName() const;
		const std::string& getTopic() const;
		const std::vector<int>& getUsers() const;
		bool isOperator(int user_fd) const;
		unsigned long getNbUsers() const;
		unsigned long getUserLimit() const;
		const std::string& getKey() const;
		
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
		bool isUserInvited(int user_fd) const;
		void inviteUser(int user_fd);
		void uninviteUser(int user_fd);

		void setInviteOnly(bool status);
		void setTopicProtected(bool status);
		void setKey(const std::string& newKey);
		void setUserLimit(unsigned long limit);
		
		// User list as a formatted string for IRC responses
		std::string getUserList(const std::map<int, std::string>& nicknames) const;
};

#endif