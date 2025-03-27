NAME		= ircserv
CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -I./includes
RM			= rm -f

SRCS_DIR	= src
OBJS_DIR	= obj

SRCS		= $(SRCS_DIR)/main.cpp
OBJS		= $(OBJS_DIR)/main.o

all:		$(NAME)

$(OBJS_DIR):
			mkdir -p $(OBJS_DIR)


$(OBJS_DIR)/main.o: $(SRCS_DIR)/main.cpp | $(OBJS_DIR)
			$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
			$(RM) -r $(OBJS_DIR)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
