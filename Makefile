# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: filpedroso <filpedroso@student.42.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/28 18:27:51 by filpedroso        #+#    #+#              #
#    Updated: 2025/05/15 16:44:14 by filpedroso       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# Compilation Variables
NAME = fildefer
CC = cc
CFLAGS = # -Wextra -Wall -Werror -Wpedantic -Wconversion -Wstrict-prototypes 
CFLAGS = # -Wextra -Wall -Werror -Wpedantic -Wconversion -Wstrict-prototypes 
SRC = $(FILES)
OBJ = $(SRC:.c=.o)
LIBFT = libft/libft.a
MLX = resources/minilibx/

# Verbose flag
VB	= 0

# Check if VERBOSE is set to 1
ifeq ($(VB),1)
	AT =
else
	AT = @
endif


FILES =				\
	src/fdf.c		\
	src/parse_map.c	\
	src/tools.c		\


# Colors
BLACK		= \033[1;30m
RED			= \033[1;31m
GREEN		= \033[1;32m
YELLOW		= \033[1;33m
BLUE		= \033[1;34m
MAGENTA		= \033[1;35m
CYAN		= \033[1;36m
WHITE		= \033[1;37m
NC			= \033[0m # No Color


all: $(LIBFT) $(NAME)

%.o: %.c
	$(AT) $(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@printf "$(WHITE)Compiling libft: |"
	$(AT) $(MAKE) -C libft | awk '{printf "="; fflush()}'
	@printf "|\n"
	@printf "$(BLUE)LIBFT compiled.\n"

$(NAME): $(OBJ) $(LIBFT)
	$(AT) $(CC) $(CFLAGS) $(OBJ) -L$(MLX) -lmlx -lm $(LIBFT) -o $(NAME) -framework OpenGL -framework AppKit
	@printf "$(CYAN)fildefer compiled.\n"


clean:
	$(AT) $(MAKE) -C libft clean | awk {}
	@printf "$(WHITE)LIBFT objects deleted.\n"
	$(AT) rm -f $(OBJ)
	@printf "$(BLACK)Object files deleted.\n"

fclean: clean
	$(AT) $(MAKE) -C libft fclean | awk {}
	$(AT) rm -f $(NAME)
	@printf "$(RED)fildefer executable deleted.\n"

jclean:
	$(AT) rm -f $(OBJ)
	@printf "$(BLACK)Object files deleted.\n"
	$(AT) rm -f $(NAME)
	@printf "$(RED)fildefer executable deleted.\n"

re: fclean all

just: jclean all

.PHONY: all clean fclean re
