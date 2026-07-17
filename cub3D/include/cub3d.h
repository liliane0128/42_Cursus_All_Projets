/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:30:16 by hguo              #+#    #+#             */
/*   Updated: 2026/01/26 19:02:31 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <fcntl.h>
# include <math.h>

# include "../minilibx-linux/mlx.h"
# include "../libft/libft.h"

// 窗口大小
# define WIDTH 1200
# define HEIGHT 1000

// 精灵类型总数
# define SPRITE_TYPES 4

// minimap
# define MM_ORIGIN_X 10
# define MM_ORIGIN_Y 10
# define MM_W        170
# define MM_H        170
// 显示玩家周围多少格
# define MM_R        7

// 分数条
# define SCORE_PAD      10
# define SCORE_W        300
# define SCORE_H        34
# define SCORE_X_MARGIN 10
# define SCORE_Y_MARGIN 10
//颜色
// #define SCORE_BG      0x121217
// #define SCORE_BORDER  0xF2E85C
// #define SCORE_INNER   0x3A3A3A
// #define SCORE_FILL    0x2EE56B
// #define SCORE_TEXT    0xFFFFFF 
# define SCORE_BG      0x0B0B0F   // 极深黑（不是纯黑，眼睛更舒服）
# define SCORE_BORDER  0xFF4FA3   // 高饱和粉（主视觉）
# define SCORE_INNER   0x2A0F1F   // 深紫粉（内框/阴影）
# define SCORE_FILL    0xFF77B7   // 亮粉（进度填充）
# define SCORE_TEXT    0xFFFFFF   // 纯白（文字）
# define SCORE_BORDER_FLASH 0xFFFFFF
# define SCORE_FILL_FLASH   0xFF4FA3

/* Key definitions for Mac and Linux compatibility */
# ifdef __APPLE__
#  define KEY_ESC	53
#  define KEY_W	13
#  define KEY_S      1
#  define KEY_A      0
#  define KEY_D      2
#  define KEY_Q      12
#  define KEY_E      14
#  define KEY_LEFT   123
#  define KEY_RIGHT  124
#  define KEY_O      31
#  define KEY_MAP    46

# elif __linux__
#  define KEY_ESC    65307
#  define KEY_W      119
#  define KEY_S      115
#  define KEY_A      97
#  define KEY_D      100
#  define KEY_Q      113
#  define KEY_E      101
#  define KEY_LEFT   65361
#  define KEY_RIGHT  65363
#  define KEY_O      111
#  define KEY_MAP    109
# endif

//纹理结构
# define TEX_NO		0 //北
# define TEX_SO		1
# define TEX_WE		2
# define TEX_EA		3
# define TEX_CEIL   4
# define TEX_FLOOR  5
# define TEX_COUNT	6 //墙的面数

# define FOV 0.66

//颜色
# define MM_BG     0xF7A6C3  // 明亮草莓粉
# define MM_WALL   0xE0448A  // 鲜粉墙
# define MM_DOOR   0x9E1F5A  // 深玫红门
# define MM_PLAYER 0xFFFFFF
# define MM_SPRITE 0x00FFF0   // 亮青色
// 0x2EE6D6    薄荷绿

//texture 墙的纹理信息
typedef struct s_tex {
	void	*img;
	char	*addr;
	int		width;
	int		height;
	int		bpp;
	int		line_len;
	int		endian;
	char	*path;
}	t_tex;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}	t_img;

typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
	int		map_end_index;
}	t_map;

typedef struct s_mm_draw
{
	t_img	*img;
	int		x;
	int		y;
	int		size;
	int		color;
}	t_mm_draw;

//player
typedef struct s_player {
	char	pos;
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}	t_player;

typedef enum e_door_state
{
	DOOR_CLOSED,
	DOOR_OPENING,
	DOOR_OPEN,
	DOOR_CLOSING
}	t_door_state;

typedef struct s_stripe
{
	int		line_h;
	int		start;
	int		end;
	int		tex_x;
	double	step;
	double	tex_pos;
	t_tex	*tex;
	int		y;
}	t_stripe;

typedef struct s_door
{
	double			x;
	double			y;
	bool			is_open;
	t_img			img_closed;
	t_img			img_open;
	bool			visible;
	double			screen_x;
	int				width;
	int				height;
	double			perp_dist;
	t_door_state	state;
	double			t;
}	t_door;

typedef enum e_sprite_type
{
	SPR_PONY = 0,
	SPR_SPARKLE = 1,
	SPR_HEART = 2,
	SPR_DISCO = 3
}	t_sprite_type;

typedef struct s_sprite
{
	double			x;
	double			y;
	bool			visible;
	double			distance;
	int				screen_x;
	int				screen_y;
	int				width;
	int				height;
	double			angle;
	int				color;
	double			transform_x;
	double			transform_y;
	double			radius;
	double			phase;
	bool			found;
	int				type;
}	t_sprite;

typedef enum e_hit
{
	HIT_NONE,
	HIT_WALL,
	HIT_DOOR
}	t_hit;

// 纹理渲染
typedef struct s_ray
{
	double	ray_dir_x;
	double	ray_dir_y;
	int		side;
	double	perp_wall_dist;
	int		line_height;
	int		draw_start;
	int		draw_end;
	t_hit	hit_type;
	int		hit_map_x;
	int		hit_map_y;
	double	hit_point;
	int		has_door;
	double	door_dist;
	int		door_side;
	double	door_hit_point;
}	t_ray;

// 每一格
typedef struct s_step
{
	int		map_x;
	int		map_y;
	double	delta_x;
	double	delta_y;
	double	dist_x;
	double	dist_y;
	int		step_x;
	int		step_y;
	int		side;
}	t_step;

// Score bar
typedef struct s_rect
{
	int	x;
	int	y;
	int	w;
	int	h;
}	t_rect;

typedef struct s_bar
{
	t_rect	r;
	int		cur;
	int		total;
	int		bg;
	int		border;
	int		inner;
	int		fill;
}	t_bar;

// minimap
typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

//club
/*
** z_buffer - Stores the distance from the player to the first wall
** for each vertical screen column. It is used during sprite rendering
** to check if a sprite is in front of or behind a wall, ensuring correct
** occlusion and depth ordering.
*/
typedef struct s_club
{
	char		**file;
	void		*mlx;
	void		*win;
	t_img		img;
	t_map		map;
	t_player	player;
	t_sprite	*sprites;
	t_img		sprite_textures[SPRITE_TYPES];
	int			sprite_count;
	int			sprite_jump;
	int			found_count;
	int			game_won;
	t_img		win_img;
	int			win_loaded;
	t_door		*doors;
	t_tex		door_tex;
	t_tex		door_open_tex;
	int			door_count;
	t_tex		tex[TEX_COUNT];
	int			floor_color;
	int			ceiling_color;
	int			mouse_last_x;
	int			mouse_in_window;
	int			show_minimap;
	int			mouse_left;
	double		mouse_sens;
	double		z_buffer[WIDTH];
	int			key_w;
	int			key_a;
	int			key_s;
	int			key_d;
	int			key_left;
	int			key_right;
}	t_club;

//parsing
bool	is_valid_xpm_file(char *path);
bool	is_valid_map_line(char *line);
int		check_elements(t_club *club, char **lines);
int		get_map(t_club *club, char **file);
int		parse_color(char *line, t_club *club);
int		parse_texture(char *line, t_club *club);
int		parse_texture(char *line, t_club *club);
bool	prepare_map(t_club *club);
bool	find_sprite_position(t_club *club);
bool	find_door_position(t_club *club);
bool	is_cub_file(char *arg);
char	**read_file(const char *file);
int		parsing(t_club *club, char **file);
bool	check_map_valid(t_club *club);
bool	is_empty_line(char *line);
bool	is_map_at_the_end(t_club *club, char **file);
bool	is_valid_char(char c, bool bonus);
bool	check_valid_chars(t_club *club);
bool	check_player_count(t_club *club);
bool	check_first_last_row(t_club *club);
char	**copy_map(t_club *club);
bool	pad_map(t_club *club);
char	*pad_line(char *line, int new_len);
int		get_expanded_len(char *line);
char	*strdup_expanded(char *line);
bool	is_surrounded_by_zero(char **map, int x, int y);

// hooks.c
int		close_window(t_club *club);
int		key_hook(int keycode, t_club *club);

// hooks_util.c
int		key_press(int keycode, t_club *c);
int		key_release(int keycode, t_club *c);
void	update_player(t_club *c);

// img_utils.c
int		init_club_defaults(t_club *club);
int		init_image(t_club *club);
void	destroy_image(t_club *club);
void	put_pixel(t_img *img, int x, int y, int color);

// init_club.c
int		init_club(t_club *club);

// render_background.c
void	render_background(t_club *club);

// walls_util.c
void	select_stripe_tex(t_club *club, t_ray *r, t_stripe *st);
int		compute_stripe_tex_x(t_club *club, t_ray *r, t_tex *tex);
void	apply_door_shift(t_club *club, t_ray *r, t_stripe *st);
void	draw_stripe_column(t_club *club, int x, t_ray *r, t_stripe *st);
void	draw_wall_stripe(t_club *club, int x, t_ray *r);
int		is_wall(t_club *club, int map_x, int map_y);

// render_walls.c
void	render_walls(t_club *club);
void	init_step(t_club *club, t_ray *ray, t_step *s);
void	walk_until_wall(t_club *club, t_step *s, t_ray *ray);
void	compute_wall(t_ray *ray, t_step *s);

// move.c
void	move_forward_backward(t_club *club, double move_speed);
void	strafe_left_right(t_club *club, double move_speed);
void	rotate_player(t_club *club, double rot_speed);

// textures.c
int		load_all_textures(t_club *club);
void	destroy_textures(t_club *club);

//sprites
int		count_char_in_map(char **map, char target);
bool	init_sprits(t_club *club);

void	render_sprites(t_club *club);
bool	init_doors(t_club *club);

bool	fill_sprite_array(t_club *club);

int		count_sprites_all(char **map);
int		collide_sprite_at(t_club *club, double nx, double ny);

void	try_open_door(t_club *club);
int		load_sprites(t_club *club);

//utils.c
void	err_msg(char *msg);
void	free_array(char **arr);
void	destroy_doors(t_club *club);
void	destroy_sprites(t_club *club);
void	destroy_tex(t_club *club, t_tex *t);
void	destroy_textures_club(t_club *club);
void	destroy_map(t_club *club);
void	destroy_sprite_textures(t_club *club);
void	destroy_club(t_club *club);

// render_minimap.c

int		mm_color_for_cell(char c);
void	mm_draw_square(t_mm_draw *d);
void	mm_draw_player_pony(t_mm_draw *d, int tile);
void	mm_draw_sprite(t_mm_draw *d, int tile);
void	render_minimap(t_club *club);
int		mm_tile_color(t_club *club, int tx, int ty);

// render_loop.c
int		render_loop(t_club *club);

// mouse_bonus.c
int		mouse_press(int button, int x, int y, t_club *club);
int		mouse_release(int button, int x, int y, t_club *club);
int		mouse_move(int x, int y, t_club *club);

// doors_util.c
bool	init_doors(t_club *club);
t_door	*door_at(t_club *c, int mx, int my);
void	try_open_door(t_club *c);
int		handle_door_cell(t_club *c, t_step *s, t_ray *r);
void	draw_door_overlay(t_club *club, int x, t_ray *ray);
void	update_doors(t_club *c);

// draw_door_overlay.c
void	draw_door_overlay(t_club *club, int x, t_ray *ray);
void	update_collectibles(t_club *club);
int		load_win_banner(t_club *c);

/* rect primitives */
void	ui_draw_rect(t_img *img, t_rect r, int color);
void	ui_draw_border(t_img *img, t_rect r, int color, int t);

/* score font */
void	ui_draw_text_score(t_img *img, int pos[2], int color, int scale);
int		ui_score_label_width(int scale);

/* progress bar */
void	ui_draw_progress_bar(t_img *img, t_bar *b);

/* public */
void	render_score_ui(t_club *club);
int		init_bonus(t_club *club);
int		is_bonus(t_club *club);
bool	init_door_tex_defaults(t_club *club);
int		have_door(t_club *club);

#endif