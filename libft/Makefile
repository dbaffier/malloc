# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/09 10:47:53 by mmonier           #+#    #+#              #
#    Updated: 2019/09/18 18:40:05 by gbourgeo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc

CFLAGS = -Wall -Wextra -Werror

RM = rm -rf

NAME = libft.a

HEADER = inc/

OBJ_D = objs/

SRCS = 	ft_memset.c										\
	   	ft_bzero.c										\
	   	ft_memcpy.c										\
	   	ft_strlen.c										\
		ft_putchar.c 									\
		ft_putnbr.c 									\


OBJS = $(addprefix $(OBJ_D), $(SRCS:.c=.o))

all: $(OBJ_D) $(NAME)

$(OBJ_D):
	@mkdir -p $@

$(OBJ_D)%.o:%.c 
	$(CC) -o $@ $(CFLAGS) -I $(HEADER) -c $<

$(NAME): $(OBJS)
	ar rc $@ $^
	ranlib $@

clean:
	$(RM) $(OBJ_D)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

