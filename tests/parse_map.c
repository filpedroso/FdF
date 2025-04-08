/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:27:24 by fpedroso          #+#    #+#             */
/*   Updated: 2025/03/04 17:01:14 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/fdf.h"

int	parse_map(t_map *map, char *file_path)
{
	int	fd;
	printf("This one\n");

	fd = open(file_path, O_RDONLY);
	perror("Error: ");
	if (fd == -1)
	{
		printf("Error: could not open file\n");
		return (0);
	}
	if (!get_map_info(&map->width, &map->height, fd))
	{
		printf("Error: get map info\n");
		close(fd);
		return (0);
	}
	perror("Error: ");
	if (close(fd) == -1)
	{
		printf("Error: could not close file\n");
		return (0);
	}
	perror("Error: ");
	map->map_data = (int *)malloc(map->width * map->height * sizeof(int));
	perror("Error: ");
	if (map->map_data == NULL)
	{
		printf("Error: malloc map data\n");
		return (0);
	}
	fd = open(file_path, O_RDONLY);
	perror("Error: ");
	if (fd == -1)
	{
		printf("Error: could not open file on second time\n");
		return (0);
	}
	if (!mapfill(map, fd))
	{
		printf("Error: map fill failed\n");
		close(fd);
		return (0);
	}
	perror("Error: ");
	close(fd);
	perror("Error: ");
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
		line_len = get_line_length(fd); // remember that GNL returns NULL when EOF but also if an error occurs
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