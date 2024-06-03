# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abelov <abelov@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/22 19:11:54 by abelov            #+#    #+#              #
#    Updated: 2024/02/22 19:11:58 by abelov           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= .minitalk
CC				:= cc
INC_DIR			=  ./include
INC_FLAGS		:= -I. -I $(INC_DIR) -I/usr/include
CFLAGS			:= -Wall -Wextra -Werror -Wimplicit
OPT_FLAGS		:= -O0
DEBUG_FLAGS		:= -g3 -gdwarf-3

LIBFT_PATH		=  ./lib/ft
LIBFT			=  $(LIBFT_PATH)/build/libft.a
CFLAGS			+= $(DEBUG_FLAGS) $(INC_FLAGS) $(OPT_FLAGS)
LINK_FLAGS		:= -L $(LIBFT_PATH)/build -lft
CTAGS			:= ctags
RM				:= /bin/rm

BUILD_DIR		= build
BIN_DIR			= $(BUILD_DIR)/bin
SRC_DIR			= src
OBJ_DIR			= $(BUILD_DIR)/obj

SRC_DIR			=  src

DEBUG_FLAGS		:= -g3 -gdwarf-3

EXECS			= client server

client_SRCS =  client/client.c
client_OBJS =  $(client_SRCS:%.c=$(OBJ_DIR)/%.o)

SERVER_FS 	=  server.c
server_SRCS =  $(SERVER_FS:%.c=server/%.c) # Substitution references
server_OBJS =  $(server_SRCS:%.c=$(OBJ_DIR)/%.o)

all:		$(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
		@mkdir -p $(@D)
		$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -o $@ -c $<

$(LIBFT):
		@$(MAKE) -C $(LIBFT_PATH) -j8

# https://stackoverflow.com/questions/33639086/
# https://www.gnu.org/software/make/manual/make.html#Static-Pattern
# https://www.gnu.org/software/make/manual/make.html#Secondary-Expansion
.SECONDEXPANSION:
$(EXECS:%=$(BIN_DIR)/%): $(BIN_DIR)/% : $$(patsubst $(BIN_DIR)/%,$$(%_OBJS),$$@) $(LIBFT)
		@mkdir -p $(@D)
		echo $^
		echo $<
		$(CC) $(CFLAGS) $< -o $@ $(LINK_FLAGS)

$(NAME): $(EXECS:%=$(BIN_DIR)/%)
		cp -v $^ ./
		@touch $@

clean:
		@$(RM) -fr $(EXECS)
		@$(MAKE) -C $(LIBFT_PATH) clean

fclean: clean
		@$(RM) -fr $(NAME) $(BUILD_DIR) @$(RM) -fr $(OBJ_DIR)
		@$(MAKE) -C $(LIBFT_PATH) fclean

re: fclean all

norm:
		@norminette $(SRCS)
		@$(MAKE) -C $(LIBFT_PATH) norm

.PHONY: all clean test re norm $(EXECS)
