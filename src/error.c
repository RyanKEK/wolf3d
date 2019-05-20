/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohavryle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:52:52 by ohavryle          #+#    #+#             */
/*   Updated: 2019/04/08 13:52:54 by ohavryle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/wolf3d.h"

void	error_message(const char *str)
{
	ft_putstr(str);
	exit(42);
}

void	count_rows(t_data *data, char **av)
{
	int		fd;
	int		rows;
	char	*line;

	rows = 0;
	fd = open(av[1], O_RDONLY);
	while (get_next_line(fd, &line) > 0)
	{
		rows++;
		free(line);
	}
	close(fd);
	data->map_rows = rows;
	data->map_cols = 0;
	data->map = (char**)malloc(sizeof(char*) * rows);
}

void	create_map(char **av, int fd, t_data *data)
{
	char	*line;
	size_t	cols;
	int		i;
	int		j;

	i = -1;
	count_rows(data, av);
	while (get_next_line(fd, &line) > 0)
	{
		j = -1;
		cols = ft_strlen(line);
		if ((int)cols > data->map_cols)
			data->map_cols = (int)cols;
		data->map[++i] = (char*)malloc(sizeof(char) * cols + 1);
		ft_strcpy(data->map[i], line);
		data->map[i][cols] = '\0';
		free(line);
	}
}

void	check_map(t_data *data)
{
	int		empty_cell;
	int		i;
	int		j;
	int		previous_len;

	empty_cell = 0;
	previous_len = 9999999;
	i = -1;
	while (++i < data->map_rows)
	{
		j = -1;
		while (data->map[i][++j])
		{
			if (data->map[i][j] == '0' && ((i == 0 || i == data->map_rows - 1)
			|| (j >= previous_len) || (j == 0)))
				error_message("Looks like u forgot to put a wall somewhere\n");
			else if (data->map[i][j] == '0')
				empty_cell++;
		}
		previous_len = j - 1;
	}
	if (!empty_cell)
		error_message("Unfortunately, this map has no empty cells!\n");
}

void	is_error(int ac, char **av, t_data *data)
{
	int	fd;

	if (ac < 2)
		error_message("Usage: ./wolf3d map [texture1 texture2 ...]\n");
	else if (ac > 11)
		error_message("Sorry, but no more than 9 textures is supported :(\n");
	if ((fd = open(av[1], O_RDONLY)) < 0)
	{
		ft_putstr("No such file: ");
		ft_putstr(av[1]);
		ft_putchar('\n');
		exit(42);
	}
	else
		create_map(av, fd, data);
	check_map(data);
	close(fd);
}
