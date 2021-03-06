# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/24 00:18:28 by dbaffier          #+#    #+#              #
#    Updated: 2020/07/03 21:49:53 by dbaffier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
#
ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc.so
TEST_MAIN = main_test
LINKNAME = libft_malloc_$(HOSTTYPE).so

CFLAGS += -Wall -Werror -Wextra

LIBFT_PATH = libft
LIBFT_LIB = $(LIBFT_PATH)/libft.a
LIBFT_LINK = -L$(LIBFT_PATH) -lft
MALLOC_LINK = -L. -lft_malloc

INC_DIR = inc/
INCS = -I$(LIBFT_PATH)/inc -I$(INC_DIR)

DEPS_DIR = .deps/
DEPS = $(addprefix $(DEPS_DIR), $(SRC:.c=.d))

OBJS_DIR = objs/
OBJS = $(addprefix $(OBJS_DIR), $(SRC:.c=.o))
OBJS1 = $(addprefix $(OBJS_DIR), $(TEST:.c=.o))
OBJS2 = $(addprefix $(OBJS_DIR), $(TEST2:.c=.o))

SRC_DIR = src/
SRC =  	malloc.c		\
		tools.c			\
		block.c			\
		realloc.c		\
		free.c

TEST_DIR = demo/
TEST2 = demo.c

RED = "\033[1;38;2;225;20;20m"
ORANGE = "\033[1;38;2;255;120;10m"
YELLO = "\033[1;38;2;255;200;0m"
GREEN = "\033[1;38;2;0;170;101m"
LG = "\033[1;38;2;167;244;66m"
BLUE = "\033[1;38;2;50;150;250m"
PURPLE = "\033[1;38;2;150;75;255m"
WHITE = "\033[1;38;2;255;250;232m"

all: $(DEPS_DIR) $(OBJS_DIR) $(LIBFT_LIB) $(NAME)


$(DEPS_DIR):
	@mkdir -p $@

$(OBJS_DIR):
	@echo $(YELLO)Creating objs dir
	@mkdir -p $@

$(LIBFT_LIB):
	@echo $(LG)Making libft
	@make -C $(LIBFT_PATH)

$(NAME): $(LINKNAME)
	@echo $(ORANGE)Creating symlink
	@ln -fs libft_malloc_$(HOSTTYPE).so $(NAME)
	@echo $(LG)Making demo
	@gcc $(CFLAGS) -fsanitize=address $(LIBFT_LINK) $(MALLOC_LINK) $(INCS) $(TEST_DIR)demo.c -o demo_dump

$(LINKNAME): $(OBJS) $(OBJS1) $(OBJS2)
	@echo $(LG)Making $(LINKNAME)
	@gcc -shared  $(LIBFT_LINK) -o $@ $(OBJS) -lpthread

$(OBJS_DIR)%.o: $(TEST_DIR)%.c
	@gcc $(CFLAGS) -o $@ -c $< $(INCS)

$(OBJS_DIR)%.o: $(SRC_DIR)%.c
	@gcc -MT $@ -MMD -MP -MF $(DEPS_DIR)$*.Td $(CFLAGS) -o $@ -c $<  $(INCS)
	@mv -f $(DEPS_DIR)$*.Td $(DEPS_DIR)$*.d && touch $@

$(DEPS_DIR)%.d: ;
.PRECIOUS: $@

-include $(DEPS)

clean:
	@make clean -C $(LIBFT_PATH)
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(LIBFT_LIB)
	rm -f $(NAME)
	rm -f $(LINKNAME)
	rm -f ./demo_dump

re: fclean all


.PHONY: fclean all clean re
