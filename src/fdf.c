/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filpedroso <filpedroso@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:44:38 by fpedroso          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/04/14 15:45:30 by filpedroso       ###   ########.fr       */
=======
/*   Updated: 2025/05/07 23:47:28 by filpedroso       ###   ########.fr       */
>>>>>>> 91b915f (new ideas, color)
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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
	install_hooks(&canvas);
	mlx_loop(canvas.connection);
	destroy_canvas(&canvas);
}

void	install_hooks(t_canvas *canvas)
{
	mlx_hook(canvas->window, 2, 1L<<0, key_hub, canvas);
	//mlx_key_hook(canvas->window, key_hub, canvas);
	mlx_mouse_hook(canvas->window, key_hub, canvas);
}

int	key_hub(int keycode, t_canvas *canvas)
{
	/* printf("Keycode pressed: %d\n", keycode); */
	if (keycode == KEY_ESC)
	{
		destroy_canvas(canvas);
		exit(0);
	}
	if (keycode == ROTATE_L)
		canvas->camera.angle_y += 0.1f;
	else if (keycode == ROTATE_R)
		canvas->camera.angle_y -= 0.1f;
	else if (keycode == ROTATE_D)
		canvas->camera.angle_x += 0.1f;
	else if (keycode == ROTATE_U)
		canvas->camera.angle_x -= 0.1f;
	else if (keycode == ZOOM_IN)
		canvas->camera.scale++;
	else if (keycode == ZOOM_OUT)
		canvas->camera.scale--;
	else if (keycode == Z_MINUS)
		canvas->camera.z_mod -= 0.1f;
	else if (keycode == Z_PLUS)
		canvas->camera.z_mod += 0.1f;
	ft_memset(canvas->data_adr, 0, HEIGHT * canvas->size_line);
	fdf_hub(canvas);
	return (1);
}

void	fdf_hub(t_canvas *canvas)
{
	int	idx;
	int	width;
	int	height;

	/* printf("min: %i\n", canvas->map->z_min);
	printf("max: %i\n", canvas->map->z_max); */
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
}

void	draw_if_valid(t_canvas *canvas, int idx_a, int idx_b)
{
	t_point	a_point;
	t_point	b_point;

	if (idx_b >= (canvas->map->width * canvas->map->height))
	{
		return ;
	}
	a_point.x = screen_coord(idx_a, canvas, 'x');
	a_point.y = screen_coord(idx_a, canvas, 'y');
	a_point.z = canvas->map->map_data[idx_a];
	b_point.x = screen_coord(idx_b, canvas, 'x');
	b_point.y = screen_coord(idx_b, canvas, 'y');
	b_point.z = canvas->map->map_data[idx_b];
	draw_line(canvas, a_point, b_point);
}

void	draw_line(t_canvas *canvas, t_point a_point, t_point b_point)
{
	int	delta_x;
	int	delta_y;

	delta_x = abs(b_point.x - a_point.x);
	delta_y = abs(b_point.y - a_point.y);
	if (a_point.x > b_point.x && delta_y <= delta_x)
		swap_points(&a_point, &b_point);
	if (a_point.y > b_point.y && delta_y > delta_x)
		swap_points(&a_point, &b_point);
	if (delta_y > delta_x)
		draw_steep(canvas, a_point, b_point);
	else
		draw_shallow(canvas, a_point, b_point);
}


void	swap_points(t_point *a, t_point *b)
{
	t_point	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	draw_shallow(t_canvas *canvas, t_point a_point, t_point b_point)
{
	int	delta_x;
	int	delta_y;
	int	error;
	int incr;

	delta_x = b_point.x - a_point.x;
	delta_y = b_point.y - a_point.y;
	incr = 1;
	if (delta_y < 0)
	{
		incr = -1;
		delta_y = -delta_y;
	}
	a_point.z_step = (b_point.z - a_point.z) / abs(delta_x);
	error = (delta_y << 1) - delta_x;
	while (a_point.x <= b_point.x)
	{
		write_pixel(canvas, a_point.x, a_point.y, a_point.z);
		a_point.z += a_point.z_step;
		a_point.x++;
		if (error >= 0)
		{
			a_point.y += incr;
			error -= delta_x << 1;
		}
		error += delta_y << 1;
	}
}

void	draw_steep(t_canvas *canvas, t_point a_point, t_point b_point)
{
	int	delta_x;
	int	delta_y;
	int	error;
	int incr;

	delta_x = b_point.x - a_point.x;
	delta_y = b_point.y - a_point.y;
	incr = 1;
	if (delta_x < 0)
	{
		incr = -1;
		delta_x = -delta_x;
	}
	a_point.z_step = (b_point.z - a_point.z) / abs(delta_y);
	error = (delta_x << 1) - delta_y;
	while (a_point.y <= b_point.y)
	{
		write_pixel(canvas, a_point.x, a_point.y, a_point.z);
		a_point.z += a_point.z_step;
		a_point.y++;
		if (error >= 0)
		{
			a_point.x += incr;
			error -= delta_y << 1;
		}
		error += delta_x << 1;
	}
}


int screen_coord(int idx, t_canvas *canvas, char coord)
{
	float	z;
	float	relat_x;
	float	x_rot_y;
	float	z_rot_y;
	float	y_rot_x;

    z = canvas->map->map_data[idx] * canvas->camera.z_mod;
    relat_x = (idx % canvas->map->width) - (canvas->map->width / 2.0f);
    if (coord == 'x')
	{
        return ((int)((relat_x * cosf(canvas->camera.angle_y) - z * sinf(canvas->camera.angle_y)) 
				* canvas->camera.scale + WIDTH / 2));
	}
    z_rot_y = relat_x * sinf(canvas->camera.angle_y) + z * cosf(canvas->camera.angle_y);
    y_rot_x = ((idx / canvas->map->width) - (canvas->map->height / 2.0f)) * 
				cosf(canvas->camera.angle_x) + z_rot_y * sinf(canvas->camera.angle_x); // Step 2: Apply X-axis rotation to Y and Z_rot_y
	return (int)(y_rot_x * canvas->camera.scale + HEIGHT / 2);
}

void	write_pixel(t_canvas *canvas, int x, int y, int z)
{
	unsigned int	*pixel_adr;
	int				index;

	/* printf("Z: %i\n", z); */
	if (canvas->map->z_min == canvas->map->z_max)
		index = 0;
	else
	{
		index = (z - canvas->map->z_min) / (canvas->map->z_max - canvas->map->z_min);
		index = (int)(index * (COLOR_COUNT - 1));
		if (index < 0)
			index = 0;
		if (index >= COLOR_COUNT)
			index = COLOR_COUNT - 1;
	}
	if (x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT)
	{
		pixel_adr = (unsigned int *)(canvas->data_adr + (y * canvas->size_line + x * (canvas->bpp >> 3)));
		*pixel_adr = canvas->color_map[index];
	}
}

<<<<<<< HEAD
/* int screen_coord(int idx, t_canvas *canvas, char coord)
{
    float relat_x = (idx % canvas->map->width) - (canvas->map->width >> 1);
    float relat_y = (idx / canvas->map->width) - (canvas->map->height >> 1);
    float z = canvas->map->map_data[idx] * canvas->camera.z_mod;

=======
/* void write_pixel(t_canvas *canvas, int x, int y, int z) {
    // Normalize Z locally (e.g., scale to [0, 1] for gradient)
    float t = (z - canvas->map->z_min) / (canvas->map->z_max - canvas->map->z_min);
	t = fmaxf(0.0f, fminf(t, 1.0f));
   

    // Base color (e.g., rotating hue)
    float hue = fmodf(canvas->camera.angle_y * 10.0f, 360.0f);
    t_color base_color = hsl_to_rgb(hue, 0.7f, 0.5f);  // Vibrant base
    t_color peak_color = {255, 255, 255};              // White

    // Blend
    unsigned int r = (unsigned int)(base_color.r * (1 - t) + peak_color.r * t);
    unsigned int g = (unsigned int)(base_color.g * (1 - t) + peak_color.g * t);
    unsigned int b = (unsigned int)(base_color.b * (1 - t) + peak_color.b * t);

    // Draw pixel (with bounds check)
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        unsigned int *pixel = (unsigned int *)(canvas->data_adr + y * canvas->size_line + x * (canvas->bpp >> 3));
        *pixel = (r << 16) | (g << 8) | b;
    }
}

t_color hsl_to_rgb(float h, float s, float l) {
    float c = (1 - fabsf(2 * l - 1)) * s;
    float x = c * (1 - fabsf(fmodf(h / 60.0f, 2) - 1));
    float m = l - c / 2;
    t_color rgb;

    if (h < 60)      { rgb.r = c; rgb.g = x; rgb.b = 0; }
    else if (h < 120) { rgb.r = x; rgb.g = c; rgb.b = 0; }
    else if (h < 180) { rgb.r = 0; rgb.g = c; rgb.b = x; }
    else if (h < 240) { rgb.r = 0; rgb.g = x; rgb.b = c; }
    else if (h < 300) { rgb.r = x; rgb.g = 0; rgb.b = c; }
    else              { rgb.r = c; rgb.g = 0; rgb.b = x; }

    rgb.r = (rgb.r + m) * 255;
    rgb.g = (rgb.g + m) * 255;
    rgb.b = (rgb.b + m) * 255;
    return rgb;
} */




/* int screen_coord(int idx, t_canvas *canvas, char coord)
{
    float relat_x = (idx % canvas->map->width) - (canvas->map->width >> 1);
    float relat_y = (idx / canvas->map->width) - (canvas->map->height >> 1);
    float z = canvas->map->map_data[idx] * canvas->camera.z_mod;

>>>>>>> 91b915f (new ideas, color)
    // Step 1: Apply Y-axis rotation to X and Z
    float angle_y = canvas->camera.angle_y;
    float x_rot_y = relat_x * cosf(angle_y) - z * sinf(angle_y);
    float z_rot_y = relat_x * sinf(angle_y) + z * cosf(angle_y);

    // Step 2: Apply X-axis rotation to Y and Z_rot_y
    float angle_x = canvas->camera.angle_x;
    float y_rot_x = relat_y * cosf(angle_x) + z_rot_y * sinf(angle_x);

    // Final projection to screen
    if (coord == 'x')
        return (int)(x_rot_y * canvas->camera.scale + WIDTH / 2);
    else
        return (int)(y_rot_x * canvas->camera.scale + HEIGHT / 2);
} */

<<<<<<< HEAD
int screen_coord(int idx, t_canvas *canvas, char coord)
{
	float	z;
	float	relat_x;
	float	x_rot_y;
	float	z_rot_y;
	float	y_rot_x;

    z = canvas->map->map_data[idx] * canvas->camera.z_mod;
    relat_x = (idx % canvas->map->width) - (canvas->map->width >> 1);
    if (coord == 'x')
	{
        return ((int)((relat_x * cosf(canvas->camera.angle_y) - z * sinf(canvas->camera.angle_y)) 
				* canvas->camera.scale + WIDTH / 2));
	}
    z_rot_y = relat_x * sinf(canvas->camera.angle_y) + z * cosf(canvas->camera.angle_y);
    y_rot_x = ((idx / canvas->map->width) - (canvas->map->height >> 1)) * 
				cosf(canvas->camera.angle_x) + z_rot_y * sinf(canvas->camera.angle_x); // Step 2: Apply X-axis rotation to Y and Z_rot_y
	return (int)(y_rot_x * canvas->camera.scale + HEIGHT / 2);
}
=======
>>>>>>> 91b915f (new ideas, color)

/* int	screen_coord(int idx, t_canvas *canvas, char coord)
{
	float	relat_x;
	float	relat_y;
	float	rot_x;
	float	rot_y;
	float	z;

	z = canvas->map->map_data[idx] * canvas->camera.z_mod;
	relat_x = (idx % canvas->map->width) - (canvas->map->width >> 1);
	relat_y = (idx / canvas->map->width) - (canvas->map->height >> 1);
	rot_x = relat_x * cosf(canvas->camera.angle_y) - (z * sinf(canvas->camera.angle_y));
	rot_y = relat_y * cosf(canvas->camera.angle_x) + (z * sinf(canvas->camera.angle_x));

	if (coord == 'x')
		return (int)(rot_x * canvas->camera.scale + WIDTH / 2);
	return (int)(rot_y * canvas->camera.scale + HEIGHT / 2);
} */

/* int	screen_coord(int idx, t_canvas *canvas, char coord)
{
	float	relat_x;
	float	relat_y;
	float	rot_x;
	float	rot_y;
	float	z;

	relat_x = (idx % canvas->map->width) - (canvas->map->width >> 1);
	relat_y = (idx / canvas->map->width) - (canvas->map->height >> 1);
	rot_x = relat_x * canvas->camera.cos_x - relat_y * canvas->camera.sin_y;
	rot_y = relat_x * canvas->camera.sin_y + relat_y * canvas->camera.cos_x;

	if (coord == 'x')
		return (int)(rot_x * SCALE + WIDTH / 2);
	z = canvas->map->map_data[idx];
	return (int)((rot_y - z * canvas->camera.z_bias) * SCALE + HEIGHT / 2);
} */

/* 
// Detailed, better explained screen_coord:

int	screen_coord(int idx, t_canvas *canvas, char coord)
{
	int		x = idx % canvas->map->width;
	int		y = idx / canvas->map->width;
	int		z = canvas->map->map_data[idx];
	float	center_x = canvas->map->width / 2.0f;
	float	center_y = canvas->map->height / 2.0f;

	// Translate to origin (center), rotate, translate back
	float	rel_x = x - center_x;
	float	rel_y = y - center_y;

	float	rot_x = rel_x * cosf(canvas->camera.angle_z) - rel_y * sinf(canvas->camera.angle_z);
	float	rot_y = rel_x * sinf(canvas->camera.angle_z) + rel_y * cosf(canvas->camera.angle_z);

	if (coord == 'x')
		return (int)(rot_x * SCALE + WIDTH / 2);
	else
		return (int)((rot_y - z * canvas->camera.z_bias) * SCALE + HEIGHT / 2);
} 

*/


// finish program with esc, with proper destructions
// create hook with mask for esc (re-watch oceano's video)
// function receives everything by reference, destroys it and exits

// ChatGPTips:

/*
Below is a conceptual discussion (no code) of how you can structure the parsing and rendering steps for maximum
performance in C, formatted with a maximum width of 120 characters:

File Reading and Parsing
• Minimize I/O calls:
– Read the file in large chunks (e.g.,
	with fread/scanf in buffer mode) or even memory-map the file if it’s large,
so you pay the file I/O overhead only once or very few times.
• Convert text to numbers efficiently:
– Avoid repeated string operations (like multiple small strtok calls).
– Use lower-level parsing functions (e.g.,
	strtol/strtod) with careful pointer management so you don’t repeatedly
scan forward or re-check the same characters.
– If you know the file format is clean (no malformed input),
	you can skip some error checks.
• Store in contiguous arrays:
– As you parse, immediately store x, y, and z into tightly packed arrays (e.g.,
	struct or parallel arrays).
A single pass read → parse → store is usually optimal.
– This keeps data cache-friendly and sequential for later processing.

Data Representation in Memory
• Keep the grid in a single 2D or 1D array:
– For example, if your map size is width×height,
	store it in a float heights[width * height].
This makes linear traversals (such as applying color or running Bresenham) fast.
• Precompute color lookups if possible:
– If z-values are in a known integer (or capped float) range,
	create a precomputed color array (e.g., colorMap[z])
so you can convert z → color through a simple index or minimal transformation.
– This avoids repeatedly computing color components or calling complex math functions.

Bresenham’s Algorithm and Rendering
• Leverage integer arithmetic:
– Bresenham’s line-drawing algorithm already uses incremental integer math,
	which is efficient.
– If your x, y, and z are integers, keep them in integer form.
• Store pixel data in a contiguous image buffer:
– Use, for instance,
	a single array of RGBA or RGB pixels sized at image_width × image_height.
– When you compute a point on the line, index into this buffer with image[y
	* image_width + x] = color.
• Minimize branching:
– Excessive conditional checks in tight loops can reduce performance. Bresenham is already fairly lean.
• Use integer/fixed-point for z-based color:
– If z-values are floating-point but your color needs are discrete,
	clamp or convert them to integer indices
for quick lookups.

Cache Utilization and Memory Layout
• Ensure data is accessed linearly:
– The CPU fetches data in cache lines, so avoid “jumping around” in memory.
– Keep operations that traverse arrays in consistent order so you maximize cache locality.
• Separate reading from writing where meaningful:
– Read and parse into a final data structure (e.g.,
	a height map array) in one pass. Then apply color or line-
drawing in subsequent passes. This can sometimes help with large data sets.

Potential Optimizations
• Multi-threading:
– Split image regions (or sets of lines) among threads. Each thread processes its own data section in parallel.
– Avoid “false sharing,” where threads write to adjacent memory in the same cache line.
• Low-level optimizations:
– If you apply the same transform to many data points (e.g.,
	converting all z-values to color), you may benefit
from SIMD intrinsics (SSE/AVX).
– Bresenham itself is mostly integer-based,
	so it may already be efficient enough.
• Memory vs. precision trade-offs:
– If z is large or high precision, yet you only need discrete color steps,
	clamp/scale z to a smaller integer
range (e.g., 0–255). This shrinks your lookup table and improves performance.

Summary Strategy for Speed

Use bulk or memory-mapped file I/O to minimize overhead.
Parse and store grid data in a single pass into a compact array (e.g., heights[y
	* width + x]).
Use a precomputed color table for fast z → color lookups.
Use Bresenham with integer arithmetic,
	writing directly into a tightly packed image buffer.
Keep loops linear and consider parallelization for large workloads.
Minimize branching and maintain cache-friendly data structures. */

/* Below is a conceptual discussion (no code) of how you can structure the parsing and rendering steps for maximum performance in C:

File Reading and Parsing
• Minimize I/O calls:
– Read the file in large chunks (e.g.,
	with fread/scanf in buffer mode) or even memory-map the file if it’s large,
	so you pay the file I/O overhead only once or very few times.
• Convert text to numbers efficiently:
– Avoid repeated string operations (like multiple small strtok calls).
– Use lower-level parsing functions (e.g.,
	strtol/strtod) with careful pointer management so you don’t repeatedly scan forward or re-check the same characters.
– If you know the file format is clean (no malformed input),
	you can skip some error checks.
• Store in contiguous arrays:
– As you parse, immediately store x, y, and z into tightly packed arrays (e.g.,
	struct or parallel arrays). A single pass read → parse → store is usually optimal.
– This keeps data cache-friendly and sequential for later processing.

Data Representation in Memory
• Keep the grid in a single 2D or 1D array:
– For example, if your map is size width×height,
	store it in a float heights[width*height]. This makes linear traversals (such as applying color or running Bresenham over the grid) fast.
• Precompute color lookups if possible:
– If your z-values (height) are in a known integer or capped floating range,
	create a precomputed color table (e.g.,
	colorMap[z]) so you can convert z → color with a simple array index or a minimal transformation.
– This avoids repeatedly computing color components or calling complex math functions in a loop.

Bresenham’s Algorithm and Rendering
• Bresenham is already integer-based:
– Bresenham’s line-drawing algorithm uses incremental integer arithmetic,
	which is efficient. Ensure you keep it integer-based when possible (e.g.,
	if you’re dealing with integer x, y, and z).
• Store pixel data in a contiguous image buffer:
– For instance,
	use an array of 32-bit or 24-bit values (RGBA or RGB) sized at image_width×image_height.
– When you compute a point on the line,
	directly index into this buffer with something like image[y * image_width
	+ x] = color.
• Minimize branching:
– Inside the Bresenham loop,
	branching for each step can be expensive. Try to keep conditionals to essential ones only (typical Bresenham’s approach is already lean).
• Use integer or simple fixed-point for z-based color:
– If your z-values are floating-point,
	but you only need discrete steps for color,
	cast or clamp them to integer indices to reduce overhead.

Cache Utilization and Memory Layout
• Ensure data is accessed linearly:
– The CPU cache system fetches data in chunks (cache lines),
	so if you traverse in a way that “jumps around” in memory,
	you lose cache efficiency.
– Try to keep your inner loops walking linearly across arrays.
• Separate reading from writing to avoid unnecessary flushes:
– Complete all your parsing into a final data structure (e.g.,
	a height map array),
	then do separate passes for color assignment and line drawing.
– This can sometimes be more efficient than interleaving operations if the data is large.

Potential Optimizations
• Multi-threading:
– Split the image region or set of lines among threads. Each thread processes its own section of data in parallel. Just be sure you don’t cause false sharing (where threads write to adjacent memory addresses in the same cache line).
• Low-level optimizations:
– In some cases,
	using SIMD intrinsics (SSE/AVX) can help if you are applying the same transformation on many data points (e.g.,
	if you do a final pass to convert all z values to color).
– Be mindful that Bresenham itself is mostly integer logic,
	so it’s already quite efficient.

Memory vs. Precision Trade-offs
• If z is large or high precision and you only need discrete color steps,
	clamp or scale z to a smaller range (e.g. 0–255). This shrinks your lookup table and quickens color indexing.
• If you need full floating precision for line-data computation,
	keep that in a parallel array,
	but for color mapping store an integer-friendly version.

Summary Strategy for Speed

Read the entire file (or large chunks) in one go using buffered or memory-mapped I/O.
In a single pass, parse each triple (x, y,
	z) directly into a compact array (e.g., heights[y * width
	+ x]) and store integer-friendly z-values if possible.
Use a precomputed color table for lookup so you don’t repeatedly compute color on the fly.
Implement Bresenham on this array,
	writing into a tightly packed image buffer with minimal branching and integer arithmetic.
Keep loops linear, minimize pointer overhead,
	and consider parallelization if the data is large enough to benefit.
These techniques will help you keep cache hits high,
	reduce overhead from string parsing and repeated function calls,
	and let the CPU spend its cycles on the actual line drawing rather than on data manipulation.

	*/