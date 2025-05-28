/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filpedroso <filpedroso@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:08:37 by filpedroso        #+#    #+#             */
/*   Updated: 2025/05/28 18:19:21 by filpedroso       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	free_map(t_map *map)
{
	if (map->map_data)
		free(map->map_data);
	if (map)
		free(map);
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
	int	len;

	if (num == INT_MIN)
		return (11);
	if (num < 0)
		num = -num;
	len = 1;
	while (num >= 10)
	{
		num /= 10;
		len++;
	}
	return (len);
}

void	get_z_reach(t_canvas *canvas)
{
	int	z;
	int	map_size;
	int	i;

	if (!canvas->map || !canvas->map->map_data)
		return ;
	map_size = canvas->map->width * canvas->map->height;
	canvas->map->z_max = INT_MIN;
	canvas->map->z_min = INT_MAX;
	i = -1;
	while (++i < map_size)
	{
		z = canvas->map->map_data[i];
		if (z > canvas->map->z_max)
			canvas->map->z_max = z;
		if (z < canvas->map->z_min)
			canvas->map->z_min = z;
	}
	if (canvas->map->z_max == canvas->map->z_min)
		canvas->map->z_max = canvas->map->z_min + 1;
}
