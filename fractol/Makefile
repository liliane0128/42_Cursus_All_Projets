NAME = fractol

SRC = 	src/main.c \
		src/draw.c \
		src/mandelbrot.c \
		src/julia.c \
		src/tricorn.c \
		src/hook.c \
		src/utils.c \
		src/init.c

OBJ_DIR = obj
OBJ = $(SRC:src/%.c=$(OBJ_DIR)/%.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2 -Iincludes -Iincludes/libft -Iincludes/minilibx

MLX_DIR = includes/minilibx
MLX_LIB = $(MLX_DIR)/libmlx.a
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

LIBFT_PATH = includes/libft
LIBFT = $(LIBFT_PATH)/libft.a

all: $(MLX_LIB) $(LIBFT) $(NAME)

$(MLX_LIB):
	$(MAKE) -C $(MLX_DIR)

$(LIBFT):
	$(MAKE) -C $(LIBFT_PATH)

$(NAME): $(OBJ) $(MLX_LIB) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(MLX_FLAGS) $(LIBFT)

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_PATH) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all clean fclean re
