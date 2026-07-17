# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/30 12:25:56 by shazhu            #+#    #+#              #
#    Updated: 2025/11/21 12:57:20 by lilwang          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -O0
NAME = minishell

INCLUDES = -I./includes -I./libft

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC_DIR = src
BUILTINS_DIR = $(SRC_DIR)/builtin
EXEC_DIR = $(SRC_DIR)/exec
PARSER_DIR = $(SRC_DIR)/parsing
SIGNAL_DIR = $(SRC_DIR)/signal
UTILS_DIR = $(SRC_DIR)/utils
EXPANDER_DIR = $(SRC_DIR)/expander
OBJDIR = obj

BLUE=\033[1;34m
RESET=\033[0m

SRC = \
	$(SRC_DIR)/main.c \
	$(BUILTINS_DIR)/echo.c \
	$(BUILTINS_DIR)/cd.c \
	$(BUILTINS_DIR)/pwd.c \
	$(BUILTINS_DIR)/export.c \
	$(BUILTINS_DIR)/unset.c \
	$(BUILTINS_DIR)/env.c \
	$(BUILTINS_DIR)/env_util.c \
	$(BUILTINS_DIR)/exit.c \
	$(EXEC_DIR)/ast_exec.c \
	$(EXEC_DIR)/exec_pipe.c \
	$(EXEC_DIR)/exec_cmd.c \
	$(EXEC_DIR)/exec_redir.c \
	$(EXEC_DIR)/utils.c \
	$(EXEC_DIR)/utils2.c \
	$(EXEC_DIR)/get_path.c \
	$(EXPANDER_DIR)/expander_var.c \
	$(EXPANDER_DIR)/expander_utils.c \
	$(EXPANDER_DIR)/expander_utils2.c \
	$(EXPANDER_DIR)/wildcards.c \
	$(SIGNAL_DIR)/signal.c \
	$(SIGNAL_DIR)/signal_handler.c \
	$(PARSER_DIR)/cmd_init.c \
	$(PARSER_DIR)/parser.c \
	$(PARSER_DIR)/parser_redirs.c \
	$(PARSER_DIR)/parse_utils.c \
	$(PARSER_DIR)/heredoc.c \
	$(PARSER_DIR)/heredoc_utils.c \
	$(PARSER_DIR)/quote.c \
	$(PARSER_DIR)/tokenizer.c \
	$(PARSER_DIR)/token_utils.c \
	$(PARSER_DIR)/token_utils2.c \
	$(PARSER_DIR)/ast_parser.c \
	$(PARSER_DIR)/ast_util.c \
	$(UTILS_DIR)/free.c \
	$(UTILS_DIR)/error.c

OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJDIR)/%.o,$(SRC))

all: $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(INCLUDES) $^ -lreadline -o $(NAME)
	@echo "$(BLUE) /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\ "
	@echo "( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )"
	@echo " > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ < "
	@echo " /\_/\     _      _  _      _  ____  _     _____ _     _        /\_/\ "
	@echo "( o.o )   / \\__/|/ \\/ \\  /|/ \\/ ___\\/ \\ /|/  __// \\   / \\      ( o.o )"
	@echo " > ^ <    | |\\/||| || |\\ ||| ||    \\| |_|||  \\  | |   | |       > ^ < "
	@echo " /\_/\    | |  ||| || | \\||| |\\___ || | |||  /_ | |_/\| |_/\    /\_/\ "
	@echo "( o.o )   \\_/  \\|\\_/\\_/  \\|\\_/\\____/\\_/ \\|\\____\\\\____/\\____/   ( o.o )"
	@echo " > ^ <                                                          > ^ < "
	@echo " /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\ "
	@echo "( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )"
	@echo " > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ < $(RESET)"


$(OBJDIR)/%.o: $(SRC_DIR)/%.c ./includes/minishell.h
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJDIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME) $(LIBFT)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
