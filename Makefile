# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nrontey <nrontey@student.42angouleme.fr    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/14 23:42:46 by mbico             #+#    #+#              #
#    Updated: 2025/04/17 00:13:19 by nrontey          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++
CFLAGS = -std=c++98 -Wall -Werror -Wextra
NAME = irc

SRCS = $(shell find ./srcs -name '*.cpp')
HDRS = ./hdrs
OBJS_DIR = ./obj
OBJS = $(SRCS:./srcs/%.cpp=$(OBJS_DIR)/%.o)

RED = $(shell tput setaf 1)
GREEN = $(shell tput setaf 2)
YELLOW = $(shell tput setaf 3)
BLUE = $(shell tput setaf 4)
PURPLE = $(shell tput setaf 5)
NC = $(shell tput sgr0)

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(BLUE)Linking objects...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -I $(HDRS)
	@echo "$(GREEN)$(NAME) compilation successful!$(NC)"

$(OBJS_DIR)/%.o: ./srcs/%.cpp
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling $(notdir $<)...$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(HDRS)

clean:
	@if [ -d $(OBJS_DIR) ]; then \
		echo "$(RED)Removing object files...$(NC)"; \
		rm -rf $(OBJS_DIR); \
	else \
		echo "$(YELLOW)No object files to remove.$(NC)"; \
	fi

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(NC)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
