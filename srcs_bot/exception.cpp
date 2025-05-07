/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 15:38:51 by mbico             #+#    #+#             */
/*   Updated: 2025/05/07 02:28:39 by mateo            ###   ########.fr       */
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

const char	*errEx::handCardOverflowException::what() const throw() {
	return ("Execution Error: handCardOverflowException");
}
