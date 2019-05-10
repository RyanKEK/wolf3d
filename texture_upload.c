/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_upload.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohavryle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/09 14:46:37 by ohavryle          #+#    #+#             */
/*   Updated: 2019/05/09 14:46:38 by ohavryle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	compass_facing(t_data *data)
{
	int bpp;
	int endian;
	int size_line;

	data->tx_ptrs = malloc(sizeof(void*) * 4);
	data->tx_datas = malloc(sizeof(int*) * 4);
	data->tx_ptrs[0] = mlx_xpm_file_to_image(data->mlx_ptr, "txs/dark_wolf_wall", &data->w[0], &data->h[0]);
	data->tx_datas[0] = (int*)mlx_get_data_addr(data->tx_ptrs[0], &bpp, &size_line, &endian);
	data->tx_ptrs[1] = mlx_xpm_file_to_image(data->mlx_ptr, "txs/wolf_wall", &data->w[1], &data->h[1]);
	data->tx_datas[1] = (int*)mlx_get_data_addr(data->tx_ptrs[1], &bpp, &size_line, &endian);
	data->tx_ptrs[2] = mlx_xpm_file_to_image(data->mlx_ptr, "txs/wolf_logo_dark", &data->w[2], &data->h[2]);
	data->tx_datas[2] = (int*)mlx_get_data_addr(data->tx_ptrs[2], &bpp, &size_line, &endian);
	data->tx_ptrs[3] = mlx_xpm_file_to_image(data->mlx_ptr, "txs/wolf_logo", &data->w[3], &data->h[3]);
	data->tx_datas[3] = (int*)mlx_get_data_addr(data->tx_ptrs[3], &bpp, &size_line, &endian);

}

void	crt_textures(char **av, int ac, t_data *data)
{
	int i;
	int bpp;
	int endian;
	int size_line;
	int j;

	i = 1;
	j = 0;
	while (++i < ac)
		if (mlx_xpm_file_to_image(data->mlx_ptr, av[i], &data->w[j], &data->h[j]))
			j++;
	data->textures = j;
	if (j > 0)
	{
		data->tx_ptrs = malloc(sizeof(void *) * j);
		data->tx_datas = malloc(sizeof(int *) * j);
		i = 1;
		j = -1;
		while (++i < ac)
		{
			j++;
			if (!(data->tx_ptrs[j] = mlx_xpm_file_to_image(data->mlx_ptr, av[i], &data->w[j], &data->h[j]))) {
				ft_putstr("There is no such file: ");
				ft_putstr(av[i]);
				ft_putchar('\n');
				j--;
				continue ;
			}
			data->tx_datas[j] = (int*)mlx_get_data_addr(data->tx_ptrs[j], &bpp, &size_line, &endian);
		}
	}
	else
		compass_facing(data);
}

void	crt_floor_and_ceiling_tex(t_data *data)
{
	int bpp;
	int endian;
	int size_line;

	data->floor_tex = malloc(sizeof(void *));
	data->floor_data = malloc(sizeof(int *));
	data->ceiling_tex = malloc(sizeof(void *));
	data->ceiling_data = malloc(sizeof(int *));
	if (!(data->floor_tex = mlx_xpm_file_to_image(data->mlx_ptr, "txs/floor.xpm", &data->floor_w, &data->floor_h)))
	{
		ft_putstr("There is no floor texture =(.\n");
		exit(42);
	}
	if (!(data->ceiling_tex = mlx_xpm_file_to_image(data->mlx_ptr, "txs/metalic_ceiling.xpm", &data->ceiling_w, &data->ceiling_h)))
	{
		ft_putstr("There is no ceiling texture =(.\n");
		exit(42);
	}
	data->floor_data = (int*)mlx_get_data_addr(data->floor_tex, &bpp, &size_line, &endian);
	data->ceiling_data = (int*)mlx_get_data_addr(data->ceiling_tex, &bpp, &size_line, &endian);
}