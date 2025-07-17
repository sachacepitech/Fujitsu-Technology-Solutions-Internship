##
## FUJITSU PROJECT, 2025
## second_project
## Version:
## 1.0
## File description:
## Makefile
## Author:
## Sacha Lemée, @sacha-lemee on Linkedin
## Junior Cybersecurity Consultant
## Created:
## ???
##

SRC =	$(addprefix src/, \
			create_enumerator.c \
			comparator.c \
			browse_device.c \
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
