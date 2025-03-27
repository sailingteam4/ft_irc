NAME		= ircserv
CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -g -I./includes
RM			= rm -f

SRCS_DIR	= src
OBJS_DIR	= obj

SRCS_FILES	= main.cpp Server.cpp

SRCS		= $(addprefix $(SRCS_DIR)/, $(SRCS_FILES))
OBJS		= $(addprefix $(OBJS_DIR)/, $(SRCS_FILES:.cpp=.o))

all:		$(NAME)

$(OBJS_DIR):
			mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp | $(OBJS_DIR)
			$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
			$(RM) -r $(OBJS_DIR)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
