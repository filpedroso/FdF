/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filpedroso <filpedroso@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:30:40 by filpedroso        #+#    #+#             */
/*   Updated: 2025/05/28 18:06:57 by filpedroso       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

size_t	get_color(float z, t_canvas *canvas)
{
	float	norm;
	int		index;

	if (canvas->map->z_max == canvas->map->z_min)
		return (0xffffff);
	norm = (float)(z - canvas->map->z_min) / (canvas->map->z_max
			- canvas->map->z_min);
	index = (int)(norm * 255.0f);
	return (canvas->color_lut[index]);
}

void	init_color_lut(t_canvas *canvas)
{
	size_t	i;
	size_t	red;
	size_t	green;
	size_t	blue;

	i = 0;
	while (i < 256)
	{
		if (i < 128)
		{
			red = i;
			green = 0;
		}
		else
		{
			red = 255;
			green = (i - 128) * 2;
		}
		blue = 255 - i;
		canvas->color_lut[i] = (red << 16) | (green << 8) | blue;
		i++;
	}
}
