/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrontey <nrontey@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 15:38:51 by mbico             #+#    #+#             */
/*   Updated: 2025/05/19 14:43:21 by nrontey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>
#include <string>

//multiDefErr
#define	ERR_NOSUCHNICK(username, target) ":" SERVER_NAME " 401 " + username + " " + target + " :No such nick/channel\r\n"
#define	ERR_NOSUCHCHANNEL(username, channel_name) ":" SERVER_NAME " 403 " + username + " " + channel_name + " :No such channel\r\n"
#define ERR_USERNOTINCHANNEL(username, target, channel_name) ":" SERVER_NAME " 441 " + username + " " + target + " " + channel_name + " :They aren't on that channel\r\n"
#define	ERR_NOTONCHANNEL(username, channel_name) ":" SERVER_NAME " 442 " + username + " " + channel_name + " :You're not on that channel\r\n"
#define	ERR_USERONCHANNEL(username, target, channel_name) ":" SERVER_NAME " 443 " + username + " " + target + " " + channel_name + " :is already on channel\r\n"
#define	ERR_NOTREGISTERED(username) ":" SERVER_NAME " 451 :You have not registered\r\n"
#define	ERR_NEEDMOREPARAMS(username, command) ":" SERVER_NAME " 461 " + username + " " + command + " :Not enough parameters\r\n"
#define	ERR_NOTCHANNELOP(username, channel_name) ":" SERVER_NAME " 482 " + username + " " + channel_name + " :You're not channel operator\r\n"

//joinErr
#define ERR_CHANNELISFULL(username, channel_name) ":" SERVER_NAME " 471 " + username + " " + channel_name + " :Cannot join channel (+l) - channel is full\r\n"
#define ERR_INVITEONLYCHAN(username, channel_name) ":" SERVER_NAME " 473 " + username + " " + channel_name + " :Cannot join channel (+i) - you must be invited\r\n"
#define	ERR_BADCHANNELKEY(username, channel_name) ":" SERVER_NAME " 475 " + username + " " + channel_name + " :Cannot join channel (+k) - bad key\r\n"

//nickErr
#define ERR_ERRONEUSNICKNAME(username) ":" SERVER_NAME " 432 * " + username + " :Error nickname: spaces are not allowed\r\n"
#define ERR_NICKNAMEINUSE(username) ":" SERVER_NAME " 433 * " + username + " :Nickname is already in use\r\n"

//privmsgErr
#define ERR_NOTSENDTOCHAN(username, target) ":" SERVER_NAME " 404 " + username + " " + target + " :Cannot send to channel\r\n"
#define	ERR_NOTEXTTOSEND ":" SERVER_NAME " 412 :No text to send\r\n"

//modeErr
#define	ERR_UNKNOWNMODE(username, channel_name, mode) ":" SERVER_NAME " 472 " + username + " " + channel_name + " " + mode + " :is unknown mode char to me\r\n"

enum	errno_irc_code {
	NO_ERROR = 0,
	BAD_CONVERSION = 1,
};

extern	errno_irc_code	errno_irc;

namespace errPsg {
	class	invalidPortException: public std::exception
	{
		const char	*what() const throw();
	};
	class	invalidPasswordException: public std::exception
	{
		const char	*what() const throw();
	};
	class	invalidArgException: public std::exception
	{
		const char	*what() const throw();
	};
}

namespace errStup {
	class	socketCouldNotCreate: public std::exception
	{
		const char	*what() const throw();
	};
	class	socketFailedToSet: public std::exception
	{
		const char	*what() const throw();
	};
	class	socketCouldNotBind: public std::exception
	{
		const char	*what() const throw();
	};
	class	socketCouldNotListen: public std::exception
	{
		const char	*what() const throw();
	};
}

namespace	errEx {
	class	selectFailed: public std::exception
	{
		const char	*what() const throw();
	};
}

bool	errCommand(int client_fd, const std::string& err);
