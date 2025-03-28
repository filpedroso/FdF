/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:44:38 by fpedroso          #+#    #+#             */
/*   Updated: 2025/02/21 17:26:03 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	init_all(t_canvas *canvas);

int	main(int argc, char **argv)
{
	t_canvas	*canvas;

	if (argc != 2)
		return (ft_printf("usage: ./fdf map.fdf"));
	if (!init_all(canvas, argv[1]))
		return (1);
	fdf_hub(canvas);
	mlx_loop(canvas->connection);
}

static void	fdf_hub(t_canvas canvas)
{
	//TODO:

		// understand how the image is ploted, to after create the dinamycs (rotate zoom etc)
		// different aproaches if the image is firstly formed then manipulated, or if it has to be formed every time it is manipulated
}

static int	init_all(t_canvas *canvas, char *map_path)
{
	canvas->map = NULL;
	canvas->image = NULL;
	canvas->connection = mlx_init();
	if (canvas->connection == NULL)
		exit(1);
	canvas->window = mlx_new_window(canvas->connection, HEIGHT, WIDTH, "FDF");
	if (canvas->window == NULL)
	{
		destroy_canvas(canvas);
		return (0);
	}
	canvas->image = mlx_new_image(canvas->connection, HEIGHT, WIDTH);
	if (canvas->image == NULL)
	{
		destroy_canvas(canvas);
		return (0);
	}
	canvas->map = parse_map(map_path);
	if (canvas->map == NULL)
	{
		destroy_canvas(canvas);
		return (0);
	}
	return (1);
}




// parse input map
	// malloc a 2d array (via get_next_line) with proper error checking (GNL from intra)
	// create function destroy_map for error checking and final
// init mlx, window and image
	// create a struct with mlx and win pointers
	// create a function that receives the struct via ref and initializes everything
	// safety measures for all inits
// fdf_hub function, receives the struct and parsed map
// finish program with esc, with proper destructions
	// create hook with mask for esc (re-watch oceano's video)
// function receives everything by reference, destroys it and exits




/* Below is a conceptual discussion (no code) of how you can structure the parsing and rendering steps for maximum
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
	and let the CPU spend its cycles on the actual line drawing rather than on data manipulation. */