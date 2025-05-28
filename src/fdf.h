/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filpedroso <filpedroso@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:44:33 by fpedroso          #+#    #+#             */
/*   Updated: 2025/05/28 20:53:13 by filpedroso       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "../libft/libft.h"
# include "../resources/minilibx/mlx.h"
# include <errno.h>
# include <math.h>

/* ************************************************************************** */
/*                                DEFINES                                     */
/* ************************************************************************** */

# define WIDTH 1000
# define HEIGHT 700
# define B_SIZE 256
# define HUD_COLOR 0xADD8E6
# define LIMIT_UP 3.1f
# define LIMIT_DN -0.004f

/* ************************************************************************** */
/*                           KEYCODE & MOUSE                                  */
/* ************************************************************************** */

# define ZOOM_IN 24   // Numpad +
# define ZOOM_OUT 27  // Numpad -
# define ROTATE_L 123 // Left arrow
# define ROTATE_R 124 // Right arrow
# define ROTATE_D 125 // Down arrow
# define ROTATE_U 126 // Up arrow
# define Z_PLUS 6     // Z
# define Z_MINUS 7    // X
# define RESET 15     // R
# define KEY_ESC 53   // ESC
# define X_CLOSE 17   // X button (Red cross window close event)

/* ************************************************************************** */
/*                           STRUCTS  &  TYPEDEFS                             */
/* ************************************************************************** */

typedef struct s_map
{
	int			*map_data;
	int			height;
	int			width;
	int			z_max;
	int			z_min;
}				t_map;

typedef struct s_camera
{
	int			scale;
	float		angle_x;
	float		angle_y;
	float		z_mod;
}				t_camera;

typedef struct s_canvas
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

typedef struct s_point
{
	int			x;
	int			y;
	int			z;
}				t_point;

typedef struct s_ab_line
{
	int			ax;
	int			ay;
	int			bx;
	int			by;
	int			dx;
	int			dy;
	int			z_a;
	int			z_b;
	float		z_step;
	int			increm;
	int			steep;
}				t_ab_line;

/* ************************************************************************** */
/*                            FUNCTION HEADERS                                */
/* ************************************************************************** */

// fdf.c
void			bresenham(t_canvas *canvas, t_ab_line *line);

// fdf_hub.c
void			fdf_hub(t_canvas *canvas);

// line.c
void			draw_line(t_canvas *canvas, t_point a_point, t_point b_point);

// parse_map.c
t_map			*parse_map(char *file_path);

// parse_map_tools.c
int				gnl_by_ref(int fd, char **line);
void			get_z_reach(t_canvas *canvas);
void			free_map(t_map *map);
int				numlen(int num);

// tools.c
void			destroy_canvas(t_canvas *canvas);
int				close_window(t_canvas *canvas);
void			reset_values(t_canvas *canvas);
void			null_canvas(t_canvas *canvas);
int				init_all(t_canvas *canvas);

// color.c
size_t			get_color(float z, t_canvas *canvas);
void			init_color_lut(t_canvas *canvas);

#endif
