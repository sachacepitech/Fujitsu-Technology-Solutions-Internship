
##
## @name second project
## @version 1.0
## @author Sacha Lemée
## @author Fujitsu Technology Solutions
## @file comparator.c
## @date 17 July 2025
##

SRC =	$(addprefix src/, \
			create_enumerator.c \
			comparator.c \
			main.c \
		)

OBJ =	$(SRC:.c=.o)

NAME =	second_project

all:	$(NAME)

CC ?= gcc

CFLAGS += -Wall -Wextra

CPPFLAGS = -iquoteinclude

LDFLAGS = -lsystemd

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
