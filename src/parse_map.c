/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filpedroso <filpedroso@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:27:24 by fpedroso          #+#    #+#             */
/*   Updated: 2025/05/18 15:14:25 by filpedroso       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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

void	free_map(t_map *map)
{
	if (map->map_data)
		free(map->map_data);
	if (map)
		free(map);
}

t_map	*get_map_info(int fd)
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
			break;
		if (line_len != map->width)
			return (0);
		(map->height)++;
	}
	map->map_data = (int *)malloc((size_t)(map->width * map->height) * sizeof(int));
	if (!map->map_data)
		return (free(map), NULL);
	return (map);
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
			ptr += numlen(map->map_data[y * map->width + x]);
			while (*ptr == ' ')
				ptr++;
			x++;
		}
		free(line);
		y++;
	}
	return (1);
}


int	get_line_length(int fd)
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

int	gnl_by_ref(int fd, char **line)
{
	char	*tmp;

	tmp = get_next_line(fd);
	if (!tmp) 
		return (0);
	*line = tmp;
	return (1);
}

int	numlen(int num)
{
	int len;

	if (num == INT_MIN)
    	return (11);
	if (num < 0)
		num = -num;
	len = 1;
	while(num >= 10)
	{
		num /= 10;
		len++;
	}
	return (len);
}

// set errno to zero
// GNL gets a line
// a function that:
	//checks for its validity (if it has one space between values, values shouldn't surpass INT_MAX)
	// get value for width (how many values we have)
	// width is a static int that compares itself with the next value (returns invalid map if not equal)
// increment height
// ft_strjoin appends it with the next
// malloc 1d int array [y * width + x] when finished checking and parsing
// fills this array with atoi in each string of nums

// ANOTHER METHOD WITH SPLIT
// errno to zero
// calls GNL
// splits line with split, returning a char** with each value as a single string -> (how safe is this?
																				// what if the map has
																				// double spaces ???)
// 