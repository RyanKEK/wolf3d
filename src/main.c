/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohavryle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 14:38:20 by ohavryle          #+#    #+#             */
/*   Updated: 2019/05/21 13:46:49 by ohavryle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../includes/wolf3d.h"

int		exit_x(void *par)
{
	par = NULL;
	exit(42);
	return (0);
}

int		loop_hook(void *param)
{
	t_data *data;

	data = (t_data*)param;
	ray_casting(data);
	crosshair(data);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img_ptr, 0, 0);
	return (0);
}

void	setup(t_data *data)
{
	int bpp;
	int size_line;
	int endian;

	data->mlx_ptr = mlx_init();
	data->win_ptr = mlx_new_window(data->mlx_ptr, WIDTH, HEIGHT, "lol");
	data->img_ptr = mlx_new_image(data->mlx_ptr, WIDTH, HEIGHT);
	data->img_data = mlx_get_data_addr(data->img_ptr,
	&bpp, &size_line, &endian);
	data->int_data = (int *)data->img_data;
	data->cell = 10000;
	data->pov = 45.0f;
	data->fov = 66;
	data->tx_index = 0;
	data->speed = data->cell / 8;
	data->floor_tex_w = 100.0;
	data->floor_tex_h = 100.0;
	data->shadows = 0;
	data->fog = 0;
	data->ppc = HEIGHT / 2;
}

void	find_empty_cell(t_data *data)
{
	int i;
	int j;

	i = -1;
	while (++i < data->map_rows)
	{
		j = -1;
		while (data->map[i][++j])
			if (data->map[i][j] == '0')
				break ;
		if (data->map[i][j] == '0')
			break ;
	}
	data->px = j * data->cell + data->cell / 2;
	data->py = i * data->cell + data->cell / 2;
}

int		main(int ac, char **av)
{
	t_data data;

	if (ft_strncmp(av[0], "./wolf3d", ft_strlen("./wolf3d")))
		return (0);
	is_error(ac, av, &data);
	setup(&data);
	crt_animation(&data);
	crt_textures(av, ac, &data);
	crt_floor_and_ceiling_tex(&data);
	find_empty_cell(&data);
	mlx_hook(data.win_ptr, 2, 4, key_hook, &data);
	mlx_loop_hook(data.mlx_ptr, loop_hook, &data);
	mlx_hook(data.win_ptr, 6, 0, motion_hook, &data);
	mlx_hook(data.win_ptr, 17, 1L << 17, exit_x, &data);
	mlx_mouse_hook(data.win_ptr, mouse_hook, &data);
	mlx_loop(data.mlx_ptr);
	return (0);
}
