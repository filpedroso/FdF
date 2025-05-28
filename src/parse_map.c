/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filpedroso <filpedroso@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:27:24 by fpedroso          #+#    #+#             */
/*   Updated: 2025/05/28 20:17:48 by filpedroso       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static t_map	*get_map_info(int fd);
static int		mapfill(t_map *map, int fd);
static void		get_map_data(t_map *map, char **ptr, int x, int y);
static int		get_line_length(int fd);

t_map	*parse_map(char *file_path)
{
	int		fd;
	t_map	*map;

	fd = open(file_path, O_RDONLY);
	if (fd == -1)
		return (perror("Error"), NULL);
	map = get_map_info(fd);
	close(fd);
	if (map == NULL)
		return (perror("Error"), NULL);
	fd = open(file_path, O_RDONLY);
	if (fd == -1)
		return (free_map(map), perror("Error"), NULL);
	if (!mapfill(map, fd))
	{
		close(fd);
		return (free_map(map), perror("Error"), NULL);
	}
	close(fd);
	return (map);
}

static t_map	*get_map_info(int fd)
{
	int		line_len;
	t_map	*map;

	map = (t_map *)malloc(sizeof(t_map));
	if (map == NULL)
		return (0);
	map->width = get_line_length(fd);
	if (map->width < 1)
		return (0);
	map->height = 1;
	while (1)
	{
		line_len = get_line_length(fd);
		if (line_len == 0)
			break ;
		if (line_len != map->width)
			return (0);
		(map->height)++;
	}
	map->map_data = (int *)malloc((size_t)(map->width * map->height)
			* sizeof(int));
	if (!map->map_data)
		return (free(map), NULL);
	return (map);
}

static int	mapfill(t_map *map, int fd)
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
			get_map_data(map, &ptr, x, y);
			x++;
		}
		free(line);
		y++;
	}
	return (1);
}

static void	get_map_data(t_map *map, char **ptr, int x, int y)
{
	map->map_data[y * map->width + x] = ft_atoi(*ptr);
	(*ptr) += numlen(map->map_data[y * map->width + x]);
	if (**ptr == ',')
	{
		while (**ptr != ' ')
			(*ptr)++;
	}
	while (**ptr == ' ')
		(*ptr)++;
}

static int	get_line_length(int fd)
{
	char	*line;
	char	**split_line;
	int		len;

	line = get_next_line(fd);
	if (!line)
		return (0);
	split_line = ft_split(line, ' ');
	if (!split_line)
	{
		free(line);
		return (0);
	}
	free(line);
	len = 0;
	while (split_line[len])
		free(split_line[len++]);
	free(split_line);
	return (len);
}
