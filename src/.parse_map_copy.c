/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:27:24 by fpedroso          #+#    #+#             */
/*   Updated: 2025/02/28 16:42:42 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	parse_map(t_map *map, char *file_path)
{
	int	fd;

	fd = open(file_path, O_RDONLY);
	if (fd == -1)
		return (0);
	if (!get_map_info(&map->width, &map->height, fd))
	{
		close(fd);
		return (0);
	}
	if (close(fd) == -1)
		return (0);
	map->map_data = (int *)malloc(map->width * map->height * sizeof(int));
	if (map->map_data == NULL)
		return (0);
	fd = open(file_path)
	if (fd == -1)
		return (0);
	if (!mapfill(map, fd))
	{
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

int	get_map_info(int *width, int *height, int fd)
{
	int	line_len;

	*width = 0;
	*width = get_line_length(fd);
	if (*width < 1)
		return (0);
	*height = 1;
	while (1)
	{
		line_len = get_line_length(fd);
		if (line_len == 0)
			break;
		if (line_len != *width)
			return (0);
		(*height)++;
	}
	return (1);
}

int	mapfill(t_map *map, int fd)
{
	char	*line;
	char	*ptr;
	int		x;
	int		y;

	y = 0;
	while (gnl_by_ref(fd, &line))
	{
		ptr = line;
		x = 0;
		while (x < map->width)
		{
			map->map_data[y * map->width + x] = ft_atoi(ptr);
			ptr += strlen(ft_itoa(map->map_data[y * map->width + x]));
			x += strlen(ft_itoa(map->map_data[y * map->width + x]));
		}
		free(line);
		y++;
	}
	return (1);
}
