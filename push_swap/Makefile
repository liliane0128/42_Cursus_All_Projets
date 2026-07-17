NAME = push_swap

CC = gcc
CFLAGS = -Wall -Wextra -Werror -Ilibft -Isrc
RM = rm -rf

SRCS =  src/main.c \
        src/index.c \
        src/op_push.c \
        src/op_rotate.c \
        src/op_rev_rotate.c \
        src/op_swap.c \
        src/parse_args.c \
        src/sort_chunks.c \
        src/sort_utils.c \
        src/utils.c \
        src/free.c



OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

libft/%.o: libft/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re