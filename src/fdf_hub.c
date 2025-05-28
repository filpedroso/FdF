/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_hub.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filpedroso <filpedroso@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:27:00 by filpedroso        #+#    #+#             */
/*   Updated: 2025/05/28 20:23:06 by filpedroso       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	draw_if_valid(t_canvas *canvas, int idx_a, int idx_b);
static int	screen_coord(int idx, t_canvas *canvas, char coord);
static void	draw_hud(t_canvas *canvas);

void	fdf_hub(t_canvas *canvas)
{
	int	idx;
	int	width;
	int	height;

	ft_memset(canvas->data_adr, 0, HEIGHT * (size_t)canvas->size_line);
	idx = 0;
	width = canvas->map->width;
	height = canvas->map->height;
	while (idx < width * height)
	{
		if ((idx % width) < (width - 1))
			draw_if_valid(canvas, idx, idx + 1);
		if ((idx / width) < (height - 1))
			draw_if_valid(canvas, idx, idx + width);
		idx++;
	}
	mlx_put_image_to_window(canvas->connection, canvas->window, canvas->image,
		0, 0);
	draw_hud(canvas);
}

static void	draw_if_valid(t_canvas *canvas, int idx_a, int idx_b)
{
	t_point	a_point;
	t_point	b_point;
	int		idx_limit;

	idx_limit = (canvas->map->width * canvas->map->height) - 1;
	if (idx_b > idx_limit || idx_a > idx_limit)
		return ;
	a_point.x = screen_coord(idx_a, canvas, 'x');
	a_point.y = screen_coord(idx_a, canvas, 'y');
	a_point.z = canvas->map->map_data[idx_a];
	b_point.x = screen_coord(idx_b, canvas, 'x');
	b_point.z = canvas->map->map_data[idx_b];
	b_point.y = screen_coord(idx_b, canvas, 'y');
	draw_line(canvas, a_point, b_point);
}

static int	screen_coord(int idx, t_canvas *canvas, char coord)
{
	float	z;
	float	relat_x;
	float	relat_y;
	float	z_rot_y;
	float	y_rot_x;

	z = canvas->map->map_data[idx] * canvas->camera.z_mod;
	relat_x = (idx % canvas->map->width) - (canvas->map->width / 2.0f);
	if (coord == 'x')
	{
		return ((int)((relat_x * cosf(canvas->camera.angle_x) - z
				* sinf(canvas->camera.angle_x)) * canvas->camera.scale
			+ WIDTH / 2));
	}
	z_rot_y = relat_x * sinf(canvas->camera.angle_x) + z
		* cosf(canvas->camera.angle_x);
	relat_y = (idx / canvas->map->width) - (canvas->map->height / 2.0f);
	y_rot_x = (relat_y) * cosf(canvas->camera.angle_y) - z_rot_y
		* sinf(canvas->camera.angle_y);
	return ((int)(y_rot_x * canvas->camera.scale + HEIGHT / 2));
}

static void	draw_hud(t_canvas *canvas)
{
	int	x;

	x = WIDTH >> 4;
	mlx_string_put(canvas->connection, canvas->window, x, 30, HUD_COLOR,
		"controls:");
	mlx_string_put(canvas->connection, canvas->window, x, 50, HUD_COLOR, " ");
	mlx_string_put(canvas->connection, canvas->window, x, 70, HUD_COLOR,
		"move:       arrow keys");
	mlx_string_put(canvas->connection, canvas->window, x, 90, HUD_COLOR,
		"zoom:       +/-");
	mlx_string_put(canvas->connection, canvas->window, x, 110, HUD_COLOR,
		"change z:   z/x");
	mlx_string_put(canvas->connection, canvas->window, x, 130, HUD_COLOR,
		"reset:      r");
	mlx_string_put(canvas->connection, canvas->window, x, 150, HUD_COLOR,
		"exit:       esc");
}
