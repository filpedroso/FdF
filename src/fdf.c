/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filpedroso <filpedroso@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:44:38 by fpedroso          #+#    #+#             */
/*   Updated: 2025/05/28 20:20:05 by filpedroso       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	key_hub(int keycode, t_canvas *canvas);
static void	update_y(int keycode, t_canvas *canvas);
static void	write_pixel(t_canvas *canvas, int x, int y, size_t color);

int	main(int argc, char **argv)
{
	t_canvas	canvas;

	if (argc != 2)
		return (ft_putstr_fd("Usage: ./fdf <map.fdf>\n", 2), 1);
	null_canvas(&canvas);
	canvas.map = parse_map(argv[1]);
	if (!canvas.map)
		return (1);
	if (!init_all(&canvas))
	{
		perror("Mlx initialization failed");
		destroy_canvas(&canvas);
		return (1);
	}
	fdf_hub(&canvas);
	mlx_hook(canvas.window, 2, 0, key_hub, &canvas);
	mlx_hook(canvas.window, 17, 0, close_window, &canvas);
	mlx_loop(canvas.connection);
	destroy_canvas(&canvas);
}

static int	key_hub(int keycode, t_canvas *canvas)
{
	if (keycode == KEY_ESC)
		return (close_window(canvas));
	if (keycode == ROTATE_U || keycode == ROTATE_D)
		update_y(keycode, canvas);
	else if (keycode == ROTATE_L)
		canvas->camera.angle_x += 0.1f;
	else if (keycode == ROTATE_R)
		canvas->camera.angle_x -= 0.1f;
	else if (keycode == ZOOM_IN)
		canvas->camera.scale++;
	else if (keycode == ZOOM_OUT)
	{
		if (canvas->camera.scale > 1)
			canvas->camera.scale--;
	}
	else if (keycode == Z_MINUS)
		canvas->camera.z_mod -= 0.1f;
	else if (keycode == Z_PLUS)
		canvas->camera.z_mod += 0.1f;
	else if (keycode == RESET)
		reset_values(canvas);
	return (fdf_hub(canvas), 1);
}

static void	update_y(int keycode, t_canvas *canvas)
{
	if (keycode == ROTATE_U)
		canvas->camera.angle_y += 0.1f;
	else if (keycode == ROTATE_D)
		canvas->camera.angle_y -= 0.1f;
	if (canvas->camera.angle_y >= LIMIT_UP)
		canvas->camera.angle_y = LIMIT_UP - 0.01f;
	else if (canvas->camera.angle_y <= LIMIT_DN)
		canvas->camera.angle_y = LIMIT_DN + 0.01f;
}

void	bresenham(t_canvas *canvas, t_ab_line *line)
{
	int		x;
	int		y;
	int		error;
	size_t	color;
	float	z_value;

	x = line->ax - 1;
	y = line->ay;
	z_value = (float)line->z_a;
	error = line->dx >> 1;
	while (++x <= line->bx)
	{
		color = get_color(z_value, canvas);
		if (line->steep)
			write_pixel(canvas, y, x, color);
		else
			write_pixel(canvas, x, y, color);
		error -= line->dy;
		if (error < 0)
		{
			y += line->increm;
			error += line->dx;
		}
		z_value += line->z_step;
	}
}

static void	write_pixel(t_canvas *canvas, int x, int y, size_t color)
{
	size_t	*pixel_adr;
	size_t	index;

	index = (size_t)(y * canvas->size_line + x * (canvas->bpp >> 3));
	if ((x >= 0) && (y >= 0) && (x < WIDTH) && (y < HEIGHT)
		&& (index < (size_t)canvas->size_line * HEIGHT))
	{
		pixel_adr = (size_t *)(canvas->data_adr + index);
		*pixel_adr = color;
	}
}
