# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nrontey <nrontey@student.42angouleme.fr    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/14 23:42:46 by mbico             #+#    #+#              #
#    Updated: 2025/04/14 18:15:25 by nrontey          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++
CFLAGS = -std=c++98 -Wall -Werror -Wextra
NAME = irc

SRCS = $(shell find ./srcs -name '*.cpp')
HDRS = ./hdrs
OBJS = $(SRCS:%.cpp=%.o)

RED = \033[0;31m
GREEN = \033[0;34m
YELLOW = \033[1;33m
PURPLE = \033[0;35m
NC = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $^ -o $(NAME) -I $(HDRS)
	@echo "$(GREEN)$(NAME) compilation successful !$(NC)"

%.o: %.cpp
	@mkdir -p $(@D)
	@echo "$(YELLOW)Compiling $(notdir $<)...$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(HDRS)

clean:
	@echo "$(RED)Removing object...$(NC)"
	@rm srcs/*.o
	@rm srcs/utils/*.o

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(NC)"
	@rm -f $(NAME)
	
re: fclean all

.PHONY: all clean fclean re
