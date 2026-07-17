NAME = cub3D

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude -Iminilibx

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

MLX_DIR = minilibx-linux
MLX = $(MLX_DIR)/libmlx.a
MLX_FLAGS = -lX11 -lXext -lm

OBJDIR = obj
SRC_DIR = src

SRC =	$(SRC_DIR)/main.c \
		$(SRC_DIR)/parsing/check_elements.c \
		$(SRC_DIR)/parsing/get_map.c \
		$(SRC_DIR)/parsing/parse.c \
		$(SRC_DIR)/parsing/parse_color.c \
		$(SRC_DIR)/parsing/parse_map.c \
		$(SRC_DIR)/parsing/parse_texture.c \
		$(SRC_DIR)/parsing/prepare_map.c \
		$(SRC_DIR)/parsing/utils.c \
		$(SRC_DIR)/parsing/map_utils.c \
		$(SRC_DIR)/parsing/map_utils2.c \
		$(SRC_DIR)/sprites/init_sprites.c \
		$(SRC_DIR)/sprites/fill_sprite_array.c \
		$(SRC_DIR)/sprites/render_sprites.c \
		$(SRC_DIR)/sprites/sprite_collision.c \
		$(SRC_DIR)/sprites/render_score_ui.c \
		$(SRC_DIR)/sprites/score_bar.c \
		$(SRC_DIR)/sprites/score_font.c \
		$(SRC_DIR)/sprites/score_rect.c \
		$(SRC_DIR)/render/render_loop.c \
		$(SRC_DIR)/controls/hooks.c \
		$(SRC_DIR)/controls/hooks_util.c \
		$(SRC_DIR)/door/doors_init.c \
		$(SRC_DIR)/door/doors_run.c \
		$(SRC_DIR)/door/draw_door_overlay.c \
		$(SRC_DIR)/render/render_mouse.c \
		$(SRC_DIR)/init/init_club_defaults.c \
     	$(SRC_DIR)/init/init_club.c \
    	$(SRC_DIR)/engine/render_background.c \
		$(SRC_DIR)/engine/wall_stripe_utils.c \
		$(SRC_DIR)/engine/walls_util.c \
		$(SRC_DIR)/render/img_utils.c \
		$(SRC_DIR)/controls/move.c \
		$(SRC_DIR)/sprites/win_utils.c \
		$(SRC_DIR)/render/textures.c \
		$(SRC_DIR)/engine/render_walls.c \
		$(SRC_DIR)/engine/raycast_init_step.c \
		$(SRC_DIR)/engine/raycast_walk.c \
		$(SRC_DIR)/render/render_minimap.c \
		$(SRC_DIR)/render/render_minimap_util.c \
		$(SRC_DIR)/sprites/update_collectibles.c \
		$(SRC_DIR)/utils/error.c \
		$(SRC_DIR)/utils/free.c \
		$(SRC_DIR)/utils/destroy_club.c \
		$(SRC_DIR)/utils/destroy_club_util.c 

OBJ = $(addprefix $(OBJDIR)/,$(subst $(SRC_DIR)/,,$(SRC:.c=.o)))

all: $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(MLX) $(MLX_FLAGS) -o $(NAME)
	@echo ""
	@echo "\033[1;35m"
	@echo " ________  ___  ________   ___  __                 "
	@echo "|\   __  \|\  \|\   ___  \|\  \|\  \               "
	@echo "\ \  \|\  \ \  \ \  \\ \  \ \  \/  /|_             "
	@echo " \ \   ____\ \  \ \  \\ \  \ \   ___  \            "
	@echo "  \ \  \___|\ \  \ \  \\ \  \ \  \\ \  \           "
	@echo "   \ \__\    \ \__\ \__\\ \__\ \__\\ \__\          "
	@echo " ___\|__|  ___\|__|\|__|_\|__|\|__| \|__|  ___     "
	@echo "|\   __  \|\   __  \|\   ___  \    |\  \  /  /|    "
	@echo "\ \  \|\  \ \  \|\  \ \  \\ \  \   \ \  \/  / /    "
	@echo " \ \   ____\ \  \\\  \ \  \\ \  \   \ \    / /     "
	@echo "  \ \  \___|\ \  \\\  \ \  \\ \  \   \/  /  /      "
	@echo "   \ \__\    \ \_______\ \__\\ \__\__/  / /        "
	@echo "    \|__|     \|_______|\|__| \|__|\___/ /         "
	@echo " ________  ___       ___  ___  ___\|___|/          "
	@echo "|\   ____\|\  \     |\  \|\  \|\   __  \           "
	@echo "\ \  \___|\ \  \    \ \  \\\  \ \  \|\ /_          "
	@echo " \ \  \    \ \  \    \ \  \\\  \ \   __  \         "
	@echo "  \ \  \____\ \  \____\ \  \\\  \ \  \|\  \        "
	@echo "   \ \_______\ \_______\ \_______\ \_______\       "
	@echo "    \|_______|\|_______|\|_______|\|_______|       "
	@echo "\033[0m"

$(OBJDIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ 

depend:
	@$(CC) -MM $(SRC) -Iinclude -Iminilibx > .depend

-include .depend

clean:
	@rm -rf $(OBJDIR)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re depend
