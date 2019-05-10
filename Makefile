# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ohavryle <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/02/05 18:55:52 by ohavryle          #+#    #+#              #
#    Updated: 2019/05/09 15:40:28 by ohavryle         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc

NAME = wolf3d

SRC = main.c get_next_line.c events.c debug.c primitives.c ray_casting.c animation.c error.c texture_upload.c texturing.c

OBJ = main.o get_next_line.o events.o debug.o primitives.o ray_casting.o animation.o error.o texture_upload.o texturing.o

CFLAGS = -Wall -Wextra -Werror -g -O2

all: lib $(NAME)

lib:
	@$(MAKE) -C libft all
 $(NAME): $(OBJ)
	$(CC) -o $(NAME) -I get_next_line.h -I wolf3d.h -I /usr/local/include -I libft/include *.o -L /usr/local/lib/ -lmlx -L libft/  -lft -framework OpenGL -framework AppKit

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all
	$(MAKE) -C libft re
