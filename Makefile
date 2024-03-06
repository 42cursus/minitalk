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

NAME		:= .minitalk
CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror -Wimplicit
INC_DIR		=  ./include
BUILD_DIR	=  .out
BIN_DIR		=  $(BUILD_DIR)/bin
OBJ_DIR		=  $(BUILD_DIR)/obj
SRC_DIR		=  src
LIBFT_PATH	=  ./lib/libft
LIBFT		=  $(LIBFT_PATH)/build/libft.a
INCLUDES	:= -I. -I $(INC_DIR)
DEBUG_FLAGS	:= -g3 -gdwarf-3
CFLAGS		+= $(DEBUG_FLAGS) $(INCLUDES)
LINK_FLAGS	:= -L $(LIBFT_PATH)/build -lft
EXECS		= client server
RM 			:= rm -rf

client_SRCS =  client/client.c
client_OBJS =  $(client_SRCS:%.c=$(OBJ_DIR)/%.o)

SERVER_FS 	=  server.c
server_SRCS =  $(SERVER_FS:%.c=server/%.c) # Substitution references
server_OBJS =  $(server_SRCS:%.c=$(OBJ_DIR)/%.o)

all:		$(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
			@mkdir -pv $(@D)
			$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
			@make -C $(LIBFT_PATH) -j8

# https://stackoverflow.com/questions/33639086/
# https://www.gnu.org/software/make/manual/make.html#Static-Pattern
# https://www.gnu.org/software/make/manual/make.html#Secondary-Expansion
.SECONDEXPANSION:
$(EXECS:%=$(BIN_DIR)/%): $(BIN_DIR)/% : $$(patsubst $(BIN_DIR)/%,$$(%_OBJS),$$@) $(LIBFT)
			@mkdir -p $(@D)
			$(CC) $(CFLAGS) $^ -o $@ $(LINK_FLAGS)

$(EXECS): $(EXECS:%=$(BIN_DIR)/%)

$(BIN_DIR):
			@mkdir -p $@

$(NAME):	$(EXECS)
			@touch $@

.PHONY: all $(EXECS)