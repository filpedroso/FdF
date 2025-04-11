/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filpedroso <filpedroso@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:30:18 by fpedroso          #+#    #+#             */
/*   Updated: 2025/04/11 23:12:29 by filpedroso       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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
	canvas->camera.angle_1 = 42 * M_PI / 180;
	canvas->camera.angle_2 = 42 * M_PI / 180;
	canvas->camera.sin_angle_1 = sinf(canvas->camera.angle_1);
	canvas->camera.sin_angle_2 = sinf(canvas->camera.angle_2);
	canvas->camera.cos_angle_1 = cosf(canvas->camera.angle_1);
	canvas->camera.cos_angle_2 = cosf(canvas->camera.angle_2);
	return (1);
}
