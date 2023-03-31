# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ysakahar <ysakahar@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/28 06:16:16 by ysakahar          #+#    #+#              #
#    Updated: 2023/03/31 10:04:03 by ysakahar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:= philo
CC		:= gcc
CFLAGS	:= -Werror -Wall -Wextra -pthread
INC		:= -I ./includes/

DEBUG_MODE ?= 0
CFLAGS	+= -D DEBUG_MODE=$(DEBUG_MODE)

ifeq ($(DEBUG_MODE), 1)
	CFLAGS += -fsanitize=thread -g
endif

SRC_PATH	:=	src/
OBJ_PATH	:=	obj/

SRC			:=	create_table.c \
				free_and_errors.c \
				input_validator.c \
				grim_reaper.c \
				main.c \
				output_status.c \
				philosopher.c \
				time_operations.c

SRCS	:= $(addprefix $(SRC_PATH), $(SRC))
OBJ		:= $(SRC:.c=.o)
OBJS	:= $(addprefix $(OBJ_PATH), $(OBJ))

all: $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c | $(OBJ_PATH)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ_PATH):
	mkdir -p $@

clean:
	$(RM) -r $(OBJ_PATH)

fclean: clean
	$(RM) $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re