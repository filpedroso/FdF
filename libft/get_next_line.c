/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:56:33 by fpedroso          #+#    #+#             */
/*   Updated: 2024/12/12 18:28:30 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*linefill(char *old, char *buuf);
static void	set_buf(char *buuf, int old_len);

char	*get_next_line(int fd)
{
	static char	buuf[BUFFER_SIZE + 1];
	char		*line;
	ssize_t		rbytes;

	line = NULL;
	rbytes = 1;
	while ((fd >= 0 && BUFFER_SIZE > 0) && rbytes > 0)
	{
		if (buuf[0])
		{
			line = linefill(line, buuf);
			if (!line)
				return (NULL);
			if (line[ft_strlen_nl(line) - 1] == '\n')
				break ;
		}
		else
		{
			rbytes = read(fd, buuf, BUFFER_SIZE);
			if (rbytes < 0)
				return (free(line), NULL);
			buuf[rbytes] = '\0';
		}
	}
	return (line);
}

static char	*linefill(char *old, char *buuf)
{
	char	*line;
	int		old_len;
	int		buf_len;
	int		i;

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

static void	set_buf(char *buuf, int buf_len)
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
	int fd;
	char *next_line;

	fd = open("text.txt", O_RDONLY);
	if (fd == -1)
		perror ("error");
	while((next_line = get_next_line(0)) != NULL )
	{
		printf("%s", next_line);
		free(next_line);
	}
	close(fd);
	return (0);
} */
