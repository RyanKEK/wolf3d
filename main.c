/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohavryle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 14:38:20 by ohavryle          #+#    #+#             */
/*   Updated: 2019/04/08 14:38:22 by ohavryle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "wolf3d.h"

void    clear_window(t_data data, int r, int g, int b, int d)
{
    int x;
	int y;
	int index;

	y = -1;
	while (++y < HEIGHT)
	{
		x = -1;
		while (++x < WIDTH)
		{
			index = (x + y * WIDTH) * 4;
			data.img_data[index] = b;
			data.img_data[index + 1] = g;
			data.img_data[index + 2] = r;
			data.img_data[index + 3] = d;
		}
	}
}

void	map_reading(char **av, t_data *data)
{
    int     fd;
    char    *line;
    int     rows;
    size_t     cols;
	int i;

	i = -1;
    rows = 0;
    fd = open(av[1], O_RDONLY);
    while (get_next_line(fd, &line) > 0)
    {
        rows++;
        free(line);
    }
    data->map_rows = rows;
    data->map_cols = 0;
    close(fd);
    data->map = (char**)malloc(sizeof(char*) * rows);
    fd = open(av[1], O_RDONLY);
    while (get_next_line(fd, &line) > 0)
    {
        cols = ft_strlen(line);
        if ((int)cols > data->map_cols)
            data->map_cols = (int)cols;
        data->map[++i] = (char*)malloc(sizeof(char) * cols + 1);
        ft_strcpy(data->map[i], line);
        data->map[i][cols] = '\0';
        free(line);
    }
}

void    crosshair(t_data *data)
{
    t_point start;
    t_point end;

    set_color(data, 0, 0, 0);
    start.x = WIDTH / 2 - 15;
    start.y = HEIGHT / 2 - 1;
    end.x = WIDTH / 2 + 15;
    end.y = start.y;
    while (start.y < HEIGHT / 2 + 2)
    {
        put_line(start, end, *data);
        start.y++;
        end.y++;
    }
    start.x = WIDTH / 2 - 1;
    start.y = HEIGHT / 2 - 15;
    end.x = start.x;
    end.y = HEIGHT / 2 + 15;
    while (start.x < WIDTH / 2 + 2)
    {
        put_line(start, end, *data);
        start.x++;
        end.x++;
    }
    set_color(data, 255, 255, 255);
}

int		exit_x(void *par)
{
	par = NULL;
	exit(42);
	return (0);

}

void	rooms(t_data *data)
{
	if (data->px / data->cell_size < 20 || data->py  / data->cell_size < 15
		|| data->px / data->cell_size > 34 || data->py  / data->cell_size > 18)
		clear_window(*data, 0, 0, 0, 0);
	if (data->px / data->cell_size >= 20 && data->py  / data->cell_size >= 2
		&& data->px / data->cell_size <= 34 && data->py  / data->cell_size <= 5) {
		clear_window(*data, 0, 0, 0, 0);
	}
	else if (data->px / data->cell_size >= 20 && data->py  / data->cell_size >= 9
			 && data->px / data->cell_size <= 34 && data->py  / data->cell_size <= 12) {
		clear_window(*data, 255, 255, 255, 240);
		usleep(30000);
	}
	else if (data->px / data->cell_size >= 20 && data->py  / data->cell_size >= 15
			 && data->px / data->cell_size <= 34 && data->py  / data->cell_size <= 18)
		;
}

int		loop_hook(void *param)
{
    t_data *data;
    data = (t_data*)param;
	clear_window(*data, 0, 0, 0, 0);
	if (data->rooms)
		rooms(data);
	if (data->debug % 2 == 0)
		draw_grid(*data);
	ray_casting(data);
	crosshair(data);
    mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img_ptr, 0, 0);
    return (0);
}

int main(int ac, char **av)
{
    t_data data;
    int bpp;
    int endian;
    int size_line;
    int i = -1;
    int j;

    is_error(ac, av);
    data.mlx_ptr = mlx_init();
    data.win_ptr = mlx_new_window(data.mlx_ptr, WIDTH, HEIGHT, "lol");
    data.img_ptr = mlx_new_image(data.mlx_ptr, WIDTH, HEIGHT);
    data.img_data = mlx_get_data_addr(data.img_ptr, &bpp, &size_line, &endian);
	data.int_data = (int *)data.img_data;
    data.cell_size = 10000;
    data.debug = 1;
    data.pov = 45.0f;
    data.fov = 66;
    data.tx_index = 0;
    data.speed = data.cell_size / 6;
	data.floor_tex_w = 100.0;
	data.floor_tex_h = 100.0;
    crt_animation(&data);
    crt_textures(av, ac, &data);
    crt_floor_and_ceiling_tex(&data);
    map_reading(av, &data);
    data.shadows = 0;
    data.fog = 0;
    if (!strcmp(av[1], "rooms"))
    	data.rooms = 1;
    else
    	data.rooms = 0;
    j = -1;
    while (++i < data.map_rows)
	{
		j = -1;
		while (++j < data.map_cols)
			if (data.map[i][j] == '0')
				break;
		if (data.map[i][j] == '0')
			break;
	}
	data.px = j * data.cell_size + data.cell_size / 2;
	data.py = i * data.cell_size + data.cell_size / 2;
	data.ppc = HEIGHT / 2;
	mlx_hook(data.win_ptr, 2, 4, key_hook, &data);
    mlx_loop_hook(data.mlx_ptr, loop_hook, &data);
	mlx_hook(data.win_ptr, 6, 0, motion_hook, &data);
	mlx_hook(data.win_ptr, 17, 1L << 17, exit_x, &data);
	mlx_mouse_hook(data.win_ptr, mouse_hook, &data);
	mlx_loop(data.mlx_ptr);
    return (0);
}
