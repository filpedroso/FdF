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

size_t get_color(float z, t_canvas *canvas)
{
	float norm;
	int index;

	if (canvas->map->z_max == canvas->map->z_min)
		return 0xffffff; // Flat map fallback

	norm = (float)(z - canvas->map->z_min) / 
	       (canvas->map->z_max - canvas->map->z_min);
	index = (int)(norm * 255.0f);
	return canvas->color_lut[index];
}

void	init_color_lut(t_canvas *canvas)
{
	size_t i;

	for (i = 0; i < 256; i++)
	{
		// Basic magma-ish palette simulation
		size_t r = i < 128 ? i : 255;
		size_t g = i < 128 ? 0 : (i - 128) * 2;
		size_t b = 255 - i;
		canvas->color_lut[i] = (r << 16) | (g << 8) | b;
	}
}
