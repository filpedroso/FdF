/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:30:18 by fpedroso          #+#    #+#             */
/*   Updated: 2025/03/04 20:01:04 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	null_canvas(t_canvas *canvas)
{
	canvas->connection = NULL;
	canvas->window = NULL;
	canvas->image = NULL;
	canvas->data_adr = NULL;
	canvas->size_line = NULL;
	canvas->endian = NULL;
	canvas->bpp = NULL;
	canvas->map->map_data = NULL;
	canvas->map->width = NULL;
	canvas->map->height = NULL;
}

int	init_all(t_canvas *canvas)
{
	canvas->connection = mlx_init();
	if (canvas->connection == NULL)
		return (0);
	canvas->window = mlx_new_window(canvas->connection, HEIGHT, WIDTH, "FDF");
	if (canvas->window == NULL)
		return (0);
	canvas->image = mlx_new_image(canvas->connection, HEIGHT, WIDTH);
	if (canvas->image == NULL)
		return (0);
	canvas->data_adr = mlx_get_data_addr(&canvas->image, &canvas->bpp, &canvas->size_line, &canvas->endian);
	return (1);
}
