/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 15:38:51 by mbico             #+#    #+#             */
/*   Updated: 2025/04/17 17:21:19 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.hpp"
	
const char	*errPsg::invalidPortException::what() const throw() {
	return ("Parsing Error: invalidPortException\nUsage: ./irc <port> <password>");	
}

const char	*errPsg::invalidPasswordException::what() const throw() {
	return ("Parsing Error: invalidPasswordException\nUsage: ./irc <port> <password>");
}

const char	*errPsg::invalidArgException::what() const throw() {
	return ("Parsing Error: invalidArgException\nUsage: ./irc <port> <password>");
}
	
const char	*errStup::socketCouldNotCreate::what() const throw() {
	return ("Startup Error: socketCouldNotCreate");
}

const char	*errStup::socketFailedToSet::what() const throw() {
	return ("Startup Error: socketFailedToSet");
}

const char	*errStup::socketCouldNotBind::what() const throw() {
	return ("Startup Error: socketFailedNotBind");
}

const char	*errStup::socketCouldNotListen::what() const throw() {
	return ("Startup Error: socketCouldNotListen");
}

const char	*errEx::selectFailed::what() const throw() {
	return ("Execution Error: selectFailed");
}

