NAME = containers
CC = clang++
CFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS_DIR = ./srcs
OBJS_DIR = ./objs
SRCS =	./srcs/main.cpp

OBJS = $(addprefix $(OBJS_DIR)/, $(notdir $(SRCS:.cpp=.o)))
vpath %.cpp $(SRCS_DIR)
RM = rm -f

all: $(NAME)

$(NAME) : $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $^

$(OBJS_DIR) :
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o : %.cpp | $(OBJS_DIR)
	@$(CC) $(CFLAGS) -o $@ -c $^

clean :
	@$(RM) -r $(OBJS_DIR)

fclean : clean
	@$(RM) $(NAME)

re : fclean all


