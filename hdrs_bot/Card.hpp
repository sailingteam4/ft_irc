/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Card.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateo <mateo@42angouleme.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 23:21:25 by mateo             #+#    #+#             */
/*   Updated: 2025/05/09 17:41:35 by mateo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include "color.hpp"

enum	e_color {
	SPIKE,
	HEART,
	CLUB,
	DIAMOND,
};

enum	e_edition {
	NO_EDITION,
	FOIL,
	HOLOGRAPHIC,
	POLYCHROME,
	NEGATIVE,
};

enum	e_enhancement {
	NO_ENHANCEMENT,
	BONUS_CARD,
	MULT_CARD,
	WILD_CARD,
	GLASS_CARD,
	STEEL_CARD,
	STONE_CARD,
	GOLD_CARD,
	LUCKY_CARD,
};

enum	e_seal {
	NO_SEAL,
	GOLD_SEALS,
	RED_SEALS,
	BLUE_SEALS,
	PURPLE_SEALS,
};

# define CARD_TOP			{"╭", "─", "─", "─", "─", "─", "╮"}
# define CARD_ICON(color)	{"│", color, " ", " ", " ", " ", "│"}
# define CARD_MID(value)	{"│", " ", value[0], value[1], " ", " ",  "│"}
# define CARD_RICON(color)	{"│", " ", " ", " ", " ", color, "│"}
# define CARD_BTM			{"╰", "─", "─", "─", "─", "─", "╯"}

# define CARD_TOP_T			"╭──┈"
# define CARD_ICON_T(color)	"│" + color
# define CARD_MID_T(value)	"│ " + value
# define CARD_RICON_T		"│   "
# define CARD_BTM_T			"╰──┈"

static const std::string	value_str[] = {"  ", "A ", "2 ", "3 ", "4 ", "5 ", "6 ",\
			"7 ", "8 ", "9 ", "10", "J ", "Q ", "K "};

static const std::string	color_str[] = {HEXBLACK + "♠" + HEXRESET, HEXRED + "♥" + HEXRESET, HEXBLACK + "♣" + HEXRESET, HEXRED + "♦" + HEXRESET};

static const std::string	edition_str[] = {"", "/F", "/H", "/P", "/N"};

static const std::string	enhancement_str[] = {"", ".bo", ".mu", ".wi", ".gl", ".sl", ".sn", ".go", ".lu"};

static const std::string	seal_str[] = {"", "*G", "*R", "*B", "*P"};

#define	JACK 11
#define	QUEEN 12
#define	KING 13
#define	AS 1

class Card {
	private:
		uint8_t			_value;
		e_color			_color;
		uint32_t		_tokens;

		e_edition		_edition;
		e_enhancement	_enhancement;
		e_seal			_seal;
	public:
		Card(uint8_t value, e_color color);
		Card(const Card &cpy);
		Card	&operator=(const Card &cpy);
		Card(void);
		~Card(void);

		uint8_t			getValue() const;
		e_color			getColor() const;
		uint32_t		getTokens() const;
		e_edition		getEdition() const;
		e_enhancement	getEnhancement() const;
		e_seal			getSeal() const;

		std::vector<std::vector<std::string> >	getImg() const;
		std::vector<std::vector<std::string> >	getCovertImg() const;

		void	setCardDefault(void);
};

std::vector<Card>	defaultDeck();
