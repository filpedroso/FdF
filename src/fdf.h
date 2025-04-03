/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:44:33 by fpedroso          #+#    #+#             */
/*   Updated: 2025/03/04 15:44:14 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

#include "../resources/minilibx-linux/mlx.h"
#include "../resources/minilibx-linux/mlx_int.h"
#include "../libft/libft.h"

#define WIDTH 1200
#define HEIGHT 800
#define BUFFER_SIZE 256

typedef struct s_buffer
{
	int			buf[BUFFER_SIZE];
	int			count;
	s_buffer	*next;
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
void	null_canvas(t_canvas *canvas);
int		init_all(t_canvas *canvas);
int		gnl_by_ref(int fd, char **line);
int		parse_map(t_map *map, char *file_path);
int		get_map_info(int *width, int *height, char* path);
int		mapfill(t_map *map, int fd);
int		get_line_length(int fd);
void	null_canvas(t_canvas *canvas);
int		init_all(t_canvas *canvas, char *file_path);
int		gnl_by_ref(int fd, char **line);



#endif