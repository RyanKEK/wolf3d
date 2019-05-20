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

SRC = src/main.c src/get_next_line.c src/events.c src/utils.c src/ray_casting.c src/animation.c src/error.c src/texture_upload.c src/texturing.c src/ray_casting2.c

OBJ = src/main.o src/get_next_line.o src/events.o src/utils.o src/ray_casting.o src/animation.o src/error.o src/texture_upload.o src/texturing.o src/ray_casting2.o

CFLAGS = -Wall -Wextra -Werror -O2

all: lib $(NAME)

$(NAME):  $(OBJ) 
	  $(CC) -o $(NAME) -I incudes/get_next_line.h -I includes/wolf3d.h -I /usr/local/include -I libft/include src/*.o -L /usr/local/lib/ -lmlx -L libft/  -lft -framework OpenGL -framework AppKit

lib:
	@$(MAKE) -C libft all

clean:
	@$(MAKE) -C libft clean
	rm -f $(OBJ)

fclean: clean
	@$(MAKE) -C libft fclean
	rm -f $(NAME)

re: fclean all
