# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nrontey <nrontey@student.42angouleme.fr    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/14 23:42:46 by mbico             #+#    #+#              #
#    Updated: 2025/05/20 16:34:16 by nrontey          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++
CFLAGS = -std=c++98 -g -Wall -Werror -Wextra -o3 -march=native -funroll-loops -flto
NAME = ircserv
NAME_BOT = bot

SRCS = $(shell find ./srcs -name '*.cpp')
SRCS_BOT = $(shell find ./srcs_bot -name '*.cpp')
HDRS = ./hdrs
HDRS_BOT = ./hdrs_bot
OBJS_DIR = ./obj
OBJS_DIR_BOT = ./obj_bot
OBJS = $(SRCS:./srcs/%.cpp=$(OBJS_DIR)/%.o)
OBJS_BOT = $(SRCS_BOT:./srcs_bot/%.cpp=$(OBJS_DIR_BOT)/%.o)

RED = $(shell tput setaf 1)
GREEN = $(shell tput setaf 2)
YELLOW = $(shell tput setaf 3)
BLUE = $(shell tput setaf 4)
PURPLE = $(shell tput setaf 5)
NC = $(shell tput sgr0)

all: $(NAME)

bonus: $(NAME_BOT)

$(NAME): $(OBJS)
	@echo "$(BLUE)Linking objects...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -I $(HDRS)
	@echo "$(GREEN)$(NAME) compilation successful!$(NC)"

$(NAME_BOT): $(OBJS_BOT)
	@echo "$(BLUE)Linking objects...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS_BOT) -o $(NAME_BOT) -I $(HDRS_BOT)
	@echo "$(GREEN)$(NAME_BOT) compilation successful!$(NC)"

$(OBJS_DIR)/%.o: ./srcs/%.cpp
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling $(notdir $<)...$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(HDRS)

$(OBJS_DIR_BOT)/%.o: ./srcs_bot/%.cpp
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling $(notdir $<)...$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(HDRS_BOT)

clean:
	@if [ -d $(OBJS_DIR) ]; then \
		echo "$(RED)Removing object files...$(NC)"; \
		rm -rf $(OBJS_DIR); \
	else \
		echo "$(YELLOW)No object files to remove.$(NC)"; \
	fi
	@if [ -d $(OBJS_DIR_BOT) ]; then \
		echo "$(RED)Removing bonus object files...$(NC)"; \
		rm -rf $(OBJS_DIR_BOT); \
	else \
		echo "$(YELLOW)No bonus object files to remove.$(NC)"; \
	fi

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(NC)"
	@rm -f $(NAME)
	@echo "$(RED)Removing $(NAME_BOT)...$(NC)"
	@rm -f $(NAME_BOT)

re: fclean all

.PHONY: all bonus clean fclean re
