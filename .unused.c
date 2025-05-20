

const t_color_stop	g_stops[] = {
		{0.00f,  0,   0, 128},  // deep blue
        {0.25f,  0, 128, 255},  // light blue
        {0.50f,  0, 200,   0},  // green
        {0.75f, 160,  82,  45}, // brown
        {1.00f, 255, 255, 255}  // white
};



void get_z_reach(t_map *map)
{
    if (!map || !map->map_data)
		return;

    map->z_max = INT_MIN;
    map->z_min = INT_MAX;

    for (int i = 0; i < map->width * map->height; i++) {
        if (map->map_data[i] > map->z_max) map->z_max = map->map_data[i];
        if (map->map_data[i] < map->z_min) map->z_min = map->map_data[i];
    }

    // Handle flat maps (all Z values equal)
    if (map->z_max == map->z_min) map->z_max = map->z_min + 1;
}

/* ************************************************************************** */
/*                         screen_coord                                       */
/* ************************************************************************** */


int	screen_coord(int idx, t_canvas *canvas, char coord)
{
	float	relat_x;
	float	relat_y;
	float	rot_x;
	float	rot_y;
	float	z;

	relat_x = (idx % canvas->map->width) - (canvas->map->width >> 1);
	relat_y = (idx / canvas->map->width) - (canvas->map->height >> 1);
	rot_x = relat_x * cosf(canvas->camera.angle_x) - relat_y * sinf(canvas->camera.angle_y);
	rot_y = relat_x * sinf(canvas->camera.angle_y) + relat_y * cosf(canvas->camera.angle_x);

	if (coord == 'x')
		return (int)(rot_x * canvas->camera.scale + WIDTH / 2);
	z = (float)canvas->map->map_data[idx];
	return (int)((rot_y - z * canvas->camera.z_mod) * canvas->camera.scale + HEIGHT / 2);
}


int screen_coord(int idx, t_canvas *canvas, char coord)
{
    float relat_x = (idx % canvas->map->width) - (canvas->map->width >> 1);
    float relat_y = (idx / canvas->map->width) - (canvas->map->height >> 1);
    float z = canvas->map->map_data[idx] * canvas->camera.z_mod;

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
}


int	screen_coord(int idx, t_canvas *canvas, char coord)
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
}


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

/* ************************************************************************** */
/*                             end screen_coord                              */
/* ************************************************************************** */


void write_pixel(t_canvas *canvas, int x, int y, int z) {
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
}


void init_color_map(t_canvas *canvas, t_color color)
{
	int				num_stops;
	float			t;
	float			local_t;
	int				i;
	int				j;

    num_stops = sizeof(g_stops) / sizeof(g_stops[0]);
	i = -1;
	while(++i < COLOR_COUNT)
	{
        t = i / (float)(COLOR_COUNT - 1);
		j = -1;
		while(++j < num_stops - 1)
		{
            if (t < g_stops[j + 1].position)
                break;
		}
        local_t = (t - g_stops[j].position) / (g_stops[j + 1].position - g_stops[j].position);
        color.r = (int)((1 - local_t) * g_stops[j].r + local_t * g_stops[j + 1].r);
        color.g = (int)((1 - local_t) * g_stops[j].g + local_t * g_stops[j + 1].g);
		color.b = (int)((1 - local_t) * g_stops[j].b + local_t * g_stops[j + 1].b);
		canvas->color_map[i] = (color.b << 16) | (color.g << 8) | color.b;
	}
}


