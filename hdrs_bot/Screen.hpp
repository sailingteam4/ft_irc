/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Screen.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbico <mbico@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 21:55:03 by mbico             #+#    #+#             */
/*   Updated: 2025/05/11 17:58:22 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma	once
#include <stdint.h>
#include <vector>
#include <string>
#include <iostream>
#include "Card.hpp"
#include "Player.hpp"
#include "Table.hpp"

#define	BACKGROUND " "

class	Screen {
	private:
		uint32_t								_lenx;
		uint32_t								_leny;
		std::vector<std::vector<std::string> >	_screen;
	public:
		Screen(void);
		~Screen(void);
		std::vector<std::vector<std::string> >	getScreen(void) const;
		uint32_t					getLenX() const;
		uint32_t					getLenY() const;
		void						printScreen(void) const;
		void						displayScreen(int sockfd) const;
		void						clear();

		void	putText(std::string txt, uint32_t x, uint32_t y);
		void	putImage(std::vector<std::vector<std::string> > img, uint32_t x, uint32_t y);
		void	putCardList(std::vector<Card> hand, uint32_t x, uint32_t y, bool ind);
		void	putTable(Table table, Player player);
};

