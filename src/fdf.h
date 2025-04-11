/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filpedroso <filpedroso@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:44:33 by fpedroso          #+#    #+#             */
/*   Updated: 2025/04/11 23:04:20 by filpedroso       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

#include "../libft/libft.h"
#include <math.h>
#include <mlx.h>


/* ************************************************************************** */
/*                              SIZES DEFINES                                 */
/* ************************************************************************** */

# define WIDTH	1200
# define HEIGHT	800
# define B_SIZE	256
# define SCALE	15


/* ************************************************************************** */
/*                         KEYCODE & MOUSE DEFINES                            */
/* ************************************************************************** */

# define ZOOM_IN	69      // Numpad +
# define ZOOM_OUT	78      // Numpad -
# define ROTATE_L	65361   // Left arrow
# define ROTATE_R	65363   // Right arrow
# define ROTATE_D	65364   // Down arrow
# define ROTATE_U	65362   // Up arrow
# define PAN_L		97      // A
# define PAN_R		100     // D
# define CAM_IN		119     // W
# define CAM_OUT	115     // S
# define KEY_ESC	65307   // ESC
# define X_CLOSE	17      // X button (Red cross window close event)


/* ************************************************************************** */
/*                           STRUCTS  &  TYPEDEFS                             */
/* ************************************************************************** */

typedef struct s_map
{
	int	*map_data;
	int	height;
	int	width;
}			t_map;

typedef struct s_camera
{
	int		angle_1;
	int		angle_2;
	float	sin_angle_1;
	float	sin_angle_2;
	float	cos_angle_1;
	float	cos_angle_2;
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
}				t_canvas;

typedef void (*trigger_func)(t_canvas *);

typedef struct s_keymap
{
	int				keycode;
	trigger_func	handler;
}				t_keymap;

typedef struct s_point
{
	int x;
	int y;
	int z;
} 			t_point;

typedef struct s_buffer
{
	int				buf[BUFFER_SIZE];
	int				count;
	struct s_buffer	*next;
}			t_buffer;




/* ************************************************************************** */
/*                            FUNCTION HEADERS                                */
/* ************************************************************************** */

void	fdf_hub(t_canvas *canvas);
void	draw_if_valid(t_canvas *canvas, int idx_a, int idx_b);
void	draw_line(t_canvas *canvas, t_point a_point, t_point b_point);
void	draw_shallow(t_canvas *canvas, t_point a_point, t_point b_point);
void	draw_steep(t_canvas *canvas, t_point a_point, t_point b_point);
void	write_pixel(t_canvas *canvas, int x, int y, int z);
int		screen_coord(int idx, t_map *map, char coord);
void	swap_points(t_point *a, t_point *b);

void	install_hooks(t_canvas *canvas);
int		key_hub(int keycode, t_canvas *canvas);

t_map	*parse_map(char *file_path);
t_map	*get_map_info(int fd);
void	free_map(t_map *map);
int		mapfill(t_map *map, int fd);
int		get_line_length(int fd);
int		gnl_by_ref(int fd, char **line);
int		numlen(int num);

void	null_canvas(t_canvas *canvas);
int		init_all(t_canvas *canvas);
void	destroy_canvas(t_canvas *canvas);



#endif
