/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filpedroso <filpedroso@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:44:33 by fpedroso          #+#    #+#             */
/*   Updated: 2025/04/07 15:49:20 by filpedroso       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

#include "../resources/minilibx-linux/mlx.h"
#include "../resources/minilibx-linux/mlx_int.h"
#include "../libft/libft.h"
#include <math.h>

#define WIDTH 1200
#define HEIGHT 800
#define B_SIZE 256

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

typedef struct s_map
{
	int	*map_data;
	int	height;
	int	width;
}			t_map;


typedef struct	s_canvas
{
	void	*connection;
	void	*window;
	t_img	*image;
	char	*data_adr;
	int		size_line;
	int		endian;
	int		bpp;
	t_map	*map;
}				t_canvas;


void	fdf_hub(t_canvas *canvas);
void	draw_if_valid(t_canvas *canvas, int idx_a, int idx_b);
void	draw_line(t_canvas *canvas, t_point a_point, t_point b_point);
void	draw_shallow(t_canvas *canvas, t_point a_point, t_point b_point, int up);
void	draw_steep(t_canvas *canvas, t_point a_point, t_point b_point, int up);
void	write_pixel(t_canvas *canvas, int x, int y, int z);
int		screen_coord(int idx, t_map *map, char coord);
void	swap_points(t_point *a, t_point *b);


int	parse_map(t_map *map, char *file_path);
int	get_map_info(int *width, int *height, int fd);
int	mapfill(t_map *map, int fd);
int	get_line_length(int fd);
int	gnl_by_ref(int fd, char **line);
int	numlen(int num);


void	null_canvas(t_canvas *canvas);
int		init_all(t_canvas *canvas);
void	destroy_canvas(t_canvas *canvas);



#endif
