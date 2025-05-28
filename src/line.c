/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filpedroso <filpedroso@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:32:28 by filpedroso        #+#    #+#             */
/*   Updated: 2025/05/28 20:18:19 by filpedroso       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	init_line(t_ab_line *line, t_point *a, t_point *b);
void	swap_coords(int *a, int *b);

void	draw_line(t_canvas *canvas, t_point a, t_point b)
{
	t_ab_line	line;

	line.steep = abs(b.y - a.y) > abs(b.x - a.x);
	if (line.steep)
	{
		swap_coords(&a.x, &a.y);
		swap_coords(&b.x, &b.y);
	}
	if (a.x > b.x)
	{
		swap_coords(&a.x, &b.x);
		swap_coords(&a.y, &b.y);
	}
	init_line(&line, &a, &b);
	bresenham(canvas, &line);
}

void	swap_coords(int *a, int *b)
{
	int	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	init_line(t_ab_line *line, t_point *a, t_point *b)
{
	line->ax = a->x;
	line->ay = a->y;
	line->bx = b->x;
	line->by = b->y;
	line->dx = b->x - a->x;
	line->dy = abs(b->y - a->y);
	line->increm = a->y < b->y;
	line->z_a = a->z;
	line->z_b = b->z;
	if (line->dx)
		line->z_step = (float)(b->z - a->z) / line->dx;
	else
		line->z_step = (float)(b->z - a->z);
	if (!line->increm)
		line->increm = -1;
}
