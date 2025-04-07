/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filpedroso <filpedroso@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:44:38 by fpedroso          #+#    #+#             */
/*   Updated: 2025/04/07 15:15:32 by filpedroso       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	main(int argc, char **argv)
{
	t_canvas	*canvas;

	if (argc != 2)
	{
		ft_putstr_fd("Usage: ./fdf <map.fdf>\n", 2);
		return (1);
	}
	null_canvas(canvas);
	if (!parse_map(canvas->map, argv[1]))
	{
		perror("Invalid map or system error");
		destroy_canvas(&canvas);
		return (1);
	}
	if (!init_all(canvas))
	{
		perror("Mlx initialization failed");
		destroy_canvas(&canvas);
		return (1);
	}
	fdf_hub(&canvas);
	mlx_loop(canvas->connection);
	destroy_canvas(&canvas);
}

static void	fdf_hub(t_canvas *canvas)
{
	int	idx;
	int	width;

	idx = 0;
	width = canvas->map->width;
	while (canvas->map->map_data[idx])
	{
		if ((idx % width) < (width - 1))
			draw_if_valid(canvas, idx, idx + 1);
		if ((idx / width) < (canvas->map->height - 1))
			draw_if_valid(canvas, idx, idx + width);
		idx++;
	}
	mlx_put_image_to_window(canvas->connection, canvas->window, canvas->image,
		0, 0);
}

static void	draw_if_valid(t_canvas *canvas, int idx_a, int idx_b)
{
	t_point	a_point;
	t_point	b_point;

	if (idx_b < 0 || !canvas->map->map_data[idx_b])
	{
		return ;
	}
	a_point.x = screen_coord(idx_a, canvas->map, 'x');
	a_point.y = screen_coord(idx_a, canvas->map, 'y');
	a_point.z = canvas->map->map_data[idx_a];
	b_point.x = screen_coord(idx_b, canvas->map, 'x');
	b_point.y = screen_coord(idx_b, canvas->map, 'y');
	b_point.z = canvas->map->map_data[idx_b];
	draw_line(canvas, a_point, b_point);
}

void	draw_line(t_canvas *canvas, t_point a_point, t_point b_point)
{
	int	delta_x;
	int	delta_y;
	int	steep;
	int	up;

	if (a_point.x > b_point.x)
	{
		swap_points(&a_point, &b_point);
	}
	delta_x = b_point.x - a_point.x;
	delta_y = b_point.y - a_point.y;
	up = delta_y < 0;
	delta_y = abs(delta_y);
	steep = delta_y > delta_x;
	if (steep)
		draw_steep(canvas->data_adr, a_point, b_point, up);
	else
		draw_shallow(canvas, a_point, b_point, up);
}

void	draw_shallow(t_canvas canvas, t_point a_point, t_point b_point, int up)
{
	int	delta_x;
	int	delta_y;
	int	error;
	if (!up)
		up = -1;
	delta_x = abs(b_point.x - a_point.x);
	delta_y = b_point.y - a_point.y;
	error = (delta_x << 1) - delta_y;
	while (a_point.x <= b_point.x)
	{
		write_pixel(canvas, a_point.x, a_point.y, a_point.z);
		a_point.x ++;
		if (error >= 0)
		{
			a_point.y += up;
			error -= delta_y << 1;
		}
		error += delta_x << 1;
	}
}

void	draw_steep(t_canvas canvas, t_point a_point, t_point b_point, int up)
{
	int	delta_x;
	int	delta_y;
	int	error;

	if (!up)
		up = -1;
	delta_x = b_point.x - a_point.x;
	delta_y = abs(b_point.y - a_point.y);
	error = (delta_y << 1) - delta_x;
	while (a_point.y <= b_point.y)
	{
		write_pixel(canvas, a_point.x, a_point.y, a_point.z);
		a_point.y += up;
		if (error >= 0)
		{
			a_point.x ++;
			error -= delta_x << 1;
		}
		error += delta_y << 1;
	}
}

void	write_pixel(t_canvas *canvas, int x, int y, int z)
{
	int	color;
	int	bytes_per_pixel;

	bytes_per_pixel = canvas->image->bpp >> 3;
	z = z >> 1;
	color = (sin(z) * 127 + 128) | (sin(z + 2) * 127 + 128) | (sin(z + 4) * 127 + 128);
	if (x >= 0 && y >= 0 && x < canvas->image->width && y < canvas->image->height)
	{
		canvas->data_adr[y * canvas->image->size_line + x * bytes_per_pixel] = color;
	}
}


static int	screen_coord(int idx, t_map *map, char coord)
{
	int	x;
	int	y;
	int	z;

	x = idx % map->width;
	y = idx / map->width;
	if (coord == 'x')
	{
		return ((x - y) * cos(30 * M_PI / 180));
	}
	z = map->map_data[idx];
	return ((x + y) * sin(30 * M_PI / 180) - z);
}

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