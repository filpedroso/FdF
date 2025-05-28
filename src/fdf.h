/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filpedroso <filpedroso@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:44:33 by fpedroso          #+#    #+#             */
/*   Updated: 2025/05/28 16:35:03 by filpedroso       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

#include "../libft/libft.h"
#include <math.h>
#include <errno.h>
#include "../resources/minilibx/mlx.h"


/* ************************************************************************** */
/*                                DEFINES                                     */
/* ************************************************************************** */

# define WIDTH		1000
# define HEIGHT		700
# define B_SIZE		256
# define HUD_COLOR	0xADD8E6


/* ************************************************************************** */
/*                           KEYCODE & MOUSE                                  */
/* ************************************************************************** */

# define ZOOM_IN	24		// Numpad +
# define ZOOM_OUT	27		// Numpad -
# define ROTATE_L	123		// Left arrow
# define ROTATE_R	124		// Right arrow
# define ROTATE_D	125		// Down arrow
# define ROTATE_U	126		// Up arrow
# define Z_PLUS		6		// Z
# define Z_MINUS	7		// X
# define RESET		15		// R
# define KEY_ESC	53		// ESC
# define X_CLOSE	17		// X button (Red cross window close event)


/* ************************************************************************** */
/*                           STRUCTS  &  TYPEDEFS                             */
/* ************************************************************************** */

typedef struct	s_map
{
	int	*map_data;
	int	height;
	int	width;
	int	z_max;
	int	z_min;
}				t_map;


typedef struct s_camera
{
	int		scale;
	float	angle_x;
	float	angle_y;
	float	z_mod;
}				t_camera;


typedef struct	s_canvas
{
	void		*connection;
	void		*window;
	void		*image;
	char		*data_adr;
	int			size_line;
	int			endian;
	int			bpp;
	t_map		*map;
	t_camera	camera;
	size_t		color_lut[256];

}				t_canvas;

typedef struct	s_point
{
	int	x;
	int	y;
	int	z;
}				t_point;

typedef struct	s_ab_line
{
	int	ax;
	int	ay;
	int	bx;
	int	by;
	int	dx;
	int	dy;
	int z_a;
	int z_b;
	float	z_step;
	int	increm;
	int	steep;
}				t_ab_line;

typedef struct	s_buffer
{
	int				buf[BUFFER_SIZE];
	int				count;
	struct s_buffer	*next;
}				t_buffer;


/* ************************************************************************** */
/*                            FUNCTION HEADERS                                */
/* ************************************************************************** */

void	fdf_hub(t_canvas *canvas);
void	draw_if_valid(t_canvas *canvas, int idx_a, int idx_b);
void	draw_line(t_canvas *canvas, t_point a_point, t_point b_point);
void	draw_shallow(t_canvas *canvas, t_point a_point, t_point b_point);
void	draw_steep(t_canvas *canvas, t_point a_point, t_point b_point);
void	write_pixel(t_canvas *canvas, int x, int y, size_t color);
int		screen_coord(int idx, t_canvas *canvas, char coord);
void	swap_points(t_point *a, t_point *b);
void	reacalc_z_reach(t_map *map, int z);
void	init_line(t_ab_line	*line, t_point *a, t_point *b);
void	bresenham(t_canvas *canvas, t_ab_line *line);
int		safe_to_write(int x, int y, size_t index, int line_size);


void	install_hooks(t_canvas *canvas);
int		key_hub(int keycode, t_canvas *canvas);
void	rotate_l(t_canvas *canvas);
void	rotate_r(t_canvas *canvas);
void	rotate_d(t_canvas *canvas);
void	rotate_u(t_canvas *canvas);
void	zoom_in(t_canvas *canvas);
void	zoom_out(t_canvas *canvas);
void	z_plus(t_canvas *canvas);
void	z_minus(t_canvas *canvas);

t_map	*parse_map(char *file_path);
t_map	*get_map_info(int fd);
void	free_map(t_map *map);
int		mapfill(t_map *map, int fd);
int		get_line_length(int fd);
int		gnl_by_ref(int fd, char **line);
int		numlen(int num);
void	get_z_reach(t_canvas *canvas);
void	get_map_data(t_map *map, char **ptr, int x, int y);


void	null_canvas(t_canvas *canvas);
int		init_all(t_canvas *canvas);
void	destroy_canvas(t_canvas *canvas);
int		close_window(t_canvas *canvas);
void	draw_hud(t_canvas *canvas);
void	reset_values(t_canvas *canvas);

size_t get_color(float z, t_canvas *canvas);
void	init_color_lut(t_canvas *canvas);
void update_y(int keycode, t_canvas *canvas);



#endif
