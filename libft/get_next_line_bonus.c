/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:46:24 by fpedroso          #+#    #+#             */
/*   Updated: 2024/12/12 18:29:58 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	static char	buuf[FOPEN_MAX][BUFFER_SIZE + 1];
	char		*line;
	ssize_t		rbytes;

	line = NULL;
	rbytes = 1;
	while ((fd >= 0 && fd < FOPEN_MAX && BUFFER_SIZE > 0) && rbytes > 0)
	{
		if (buuf[fd][0])
		{
			line = linefill(line, buuf[fd]);
			if (!line)
				return (NULL);
			if (line[ft_strlen_nl(line) - 1] == '\n')
				break ;
		}
		else
		{
			rbytes = read(fd, buuf[fd], BUFFER_SIZE);
			if (rbytes < 0)
				return (free(line), NULL);
			buuf[fd][rbytes] = '\0';
		}
	}
	return (line);
}

char	*linefill(char *old, char *buuf)
{
	char		*line;
	size_t		old_len;
	size_t		buf_len;
	size_t		i;

	old_len = ft_strlen_nl(old);
	buf_len = ft_strlen_nl(buuf);
	line = (char *)malloc((old_len + buf_len + 1) * sizeof(char));
	if (!line)
		return (free(line), NULL);
	line[old_len + buf_len] = '\0';
	i = -1;
	while (++i < old_len)
		line[i] = old[i];
	free(old);
	i = -1;
	while (++i < buf_len)
		line[i + old_len] = buuf[i];
	set_buf(buuf, buf_len);
	return (line);
}

void	set_buf(char *buuf, size_t buf_len)
{
	int	i;

	i = 0;
	while (buuf[i + buf_len])
	{
		buuf[i] = buuf[i + buf_len];
		i++;
	}
	buuf[i] = '\0';
}

/* int main()
{
	int fd, fd1;
	char	*next_line;
	fd = open("text.txt", O_RDONLY);
	fd1 = open("normal.txt", O_RDONLY);

	while ((next_line = get_next_line(fd)) != NULL)
	{
		printf("%s", next_line);
		free(next_line);
	}
	close (fd);
	while((next_line = get_next_line(fd1)) != NULL)
	{
		printf("%s", next_line);
		free(next_line);
	}
	close(fd1);
	return (0);
} */