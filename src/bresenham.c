# include "fdf.h"

void	bresenham(t_canvas *canvas, t_point a_point, t_point b_point)
{
	int	error;
	int	increm;

	a_point.d_x = 
}


void	bresenham(t_canvas *canvas, t_ab_line *line)
{
	int				x;
	int				y;
	int				error;
	unsigned int	*pixel_adr;
	unsigned int	index;

	x = line->ax;
	y = line->ay;
	error = line->dx >> 1;
	while (x <= line->bx)
	{
		index = y * canvas->size_line + x * (canvas->bpp >> 3);
		if (index < canvas->size_line * HEIGHT)
		{
			pixel_adr = (unsigned int *)(canvas->data_adr + index);
			*pixel_adr = 0xffffff;
		}
		error -= line->dy;
		if (error < 0)
		{
			y += line->increm;
			error += line->dx;
		}
		x++;
	}
}
