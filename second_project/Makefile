SRC =	$(addprefix src/, \
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