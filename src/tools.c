/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filpedroso <filpedroso@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:30:18 by fpedroso          #+#    #+#             */
/*   Updated: 2025/05/18 13:11:12 by filpedroso       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "fdf.h"

const t_color_stop	g_stops[] = {
		{0.00f,  0,   0, 128},  // deep blue
        {0.25f,  0, 128, 255},  // light blue
        {0.50f,  0, 200,   0},  // green
        {0.75f, 160,  82,  45}, // brown
        {1.00f, 255, 255, 255}  // white
};

void	destroy_canvas(t_canvas *canvas)
{
	mlx_destroy_image(canvas->connection, canvas->image);
	mlx_destroy_window(canvas->connection, canvas->window);
	return ;
}

void	null_canvas(t_canvas *canvas)
{
	canvas->connection = NULL;
	canvas->window = NULL;
	canvas->image = NULL;
	canvas->data_adr = NULL;
	canvas->size_line = 0;
	canvas->endian = 0;
	canvas->bpp = 0;
	canvas->map = NULL;
}

int	init_all(t_canvas *canvas)
{
	t_color			color;

	canvas->connection = mlx_init();
	if (canvas->connection == NULL)
		return (0);
	canvas->window = mlx_new_window(canvas->connection, WIDTH, HEIGHT, "FdF");
	if (canvas->window == NULL)
		return (0);
	canvas->image = mlx_new_image(canvas->connection, WIDTH, HEIGHT);
	if (canvas->image == NULL)
		return (0);
	canvas->data_adr = mlx_get_data_addr(canvas->image, &canvas->bpp, &canvas->size_line, &canvas->endian);
	canvas->camera.angle_x = M_PI / 11;
	canvas->camera.angle_y = M_PI / 9;
	canvas->camera.scale = 25 * 200 / (canvas->map->width * canvas->map->height);
	canvas->camera.z_mod = 0.6f;
	init_color_map(canvas, color);
	return (1);
}

void init_color_map(t_canvas *canvas, t_color color)
{
	int				num_stops;
	float			t;
	float			local_t;
	int				i;
	int				j;

    num_stops = sizeof(g_stops) / sizeof(g_stops[0]);
	i = -1;
	while(++i < COLOR_COUNT)
	{
        t = i / (float)(COLOR_COUNT - 1);
		j = -1;
		while(++j < num_stops - 1)
		{
            if (t < g_stops[j + 1].position)
                break;
		}
        local_t = (t - g_stops[j].position) / (g_stops[j + 1].position - g_stops[j].position);
        color.r = (int)((1 - local_t) * g_stops[j].r + local_t * g_stops[j + 1].r);
        color.g = (int)((1 - local_t) * g_stops[j].g + local_t * g_stops[j + 1].g);
		color.b = (int)((1 - local_t) * g_stops[j].b + local_t * g_stops[j + 1].b);
		canvas->color_map[i] = (color.b << 16) | (color.g << 8) | color.b;
	}
}
