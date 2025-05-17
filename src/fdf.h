/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filpedroso <filpedroso@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:44:33 by fpedroso          #+#    #+#             */
/*   Updated: 2025/05/17 18:29:52 by filpedroso       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

#include "../libft/libft.h"
#include <math.h>
#include "../resources/minilibx/mlx.h"


/* ************************************************************************** */
/*                              SIZES DEFINES                                 */
/* ************************************************************************** */

# define WIDTH	1000
# define HEIGHT	700
# define B_SIZE	256
# define COLOR_COUNT 256


/* ************************************************************************** */
/*                         KEYCODE & MOUSE DEFINES                            */
/* ************************************************************************** */

# define ZOOM_IN	61		// Numpad +
# define ZOOM_OUT	45		// Numpad -
# define ROTATE_L	65361	// Left arrow
# define ROTATE_R	65363	// Right arrow
# define ROTATE_D	65364	// Down arrow
# define ROTATE_U	65362	// Up arrow
# define Z_PLUS		122		// Z
# define Z_MINUS	120		// X
# define PAN_L		97		// A
# define PAN_R		100		// D
# define CAM_IN		119		// W
# define CAM_OUT	115		// S
# define KEY_ESC	65307	// ESC
# define X_CLOSE	17		// X button (Red cross window close event)


/* ************************************************************************** */
/*                           STRUCTS  &  TYPEDEFS                             */
/* ************************************************************************** */

typedef struct	s_color_stop
{
    float	position;
    int		r;
    int		g;
    int		b;
}	t_color_stop;

typedef struct	s_map
{
	int	*map_data;
	int	height;
	int	width;
	int	z_min;
	int	z_max;
}				t_map;

typedef struct	s_color
{
    int r;
    int g;
    int b;
} 				t_color;

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
	int			color_map[COLOR_COUNT];
}				t_canvas;

typedef struct	s_point
{
	int	x;
	int	y;
	int z;
	int	z_step;
}				t_point;

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
void	write_pixel(t_canvas *canvas, int x, int y, int z);
int		screen_coord(int idx, t_canvas *canvas, char coord);
void	swap_points(t_point *a, t_point *b);
t_color hsl_to_rgb(float h, float s, float l);

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
void	get_z_reach(t_map *map);

void	null_canvas(t_canvas *canvas);
int		init_all(t_canvas *canvas);
void	destroy_canvas(t_canvas *canvas);
void	init_color_map(t_canvas *canvas, t_color color);



#endif
