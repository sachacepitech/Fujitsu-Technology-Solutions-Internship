## =============================================================================
# @name           druid
# @version        1.0
# @author         Sacha Lemée
# @author 		  Fujitsu Technology Solutions
# @file           Makefile
# @date           17 July 2025
# @copyright      Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0)
# 
# This file is part of the "druid" repository.
# 
# You can use, modify, and distribute this code under the terms of the
# Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0).
# See the full license at: https://creativecommons.org/licenses/by-sa/4.0/deed.fr
# ==============================================================================

SRC =	$(addprefix src/, \
			display_risk_stats_and_unknown_device.c \
			display_file.c \
			load_usb_db_from_file.c \
			handle_cli_info_flags.c \
			free_usb_db_entry.c \
			init_db_struct_and_device.c \
			init_usb_enumerator.c \
			main.c \
			scan_connected_usb_and_check_risks.c \
		)

OBJ =	$(SRC:.c=.o)

NAME =	druid

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
