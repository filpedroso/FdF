/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filpedroso <filpedroso@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:30:18 by fpedroso          #+#    #+#             */
/*   Updated: 2025/05/20 11:41:33 by filpedroso       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void destroy_canvas(t_canvas *canvas)
{
    if (canvas->window)
        mlx_destroy_window(canvas->connection, canvas->window);
    if (canvas->image)
        mlx_destroy_image(canvas->connection, canvas->image);
    if (canvas->map)
	{
        free(canvas->map->map_data);
        free(canvas->map);
    }
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
	canvas->camera.scale = 1000 / (canvas->map->width + canvas->map->height);
	canvas->camera.z_mod = 0.6f;
	return (1);
}
