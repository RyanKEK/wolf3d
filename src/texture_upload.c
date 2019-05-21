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

#include "../includes/wolf3d.h"

void	compass_facing(t_data *data)
{
	int		bpp;
	int		endian;
	int		size_line;
	int		i;
	char	*texture;

	i = -1;
	data->tx_ptrs = malloc(sizeof(void*) * 4);
	data->tx_datas = malloc(sizeof(int*) * 4);
	texture = malloc(sizeof(char) * 6);
	while (++i < 4)
	{
		ft_strncpy(texture, "txs/wall1\0", 10);
		texture[8] = i + 49;
		if (!(data->tx_ptrs[i] = mlx_xpm_file_to_image(data->mlx_ptr,
		texture, &data->w[i], &data->h[i])))
		{
			ft_putstr("Looks like some walls textures are missing=(\n");
			exit(42);
		}
		data->tx_datas[i] = (int*)mlx_get_data_addr(data->tx_ptrs[i],
		&bpp, &size_line, &endian);
	}
}

void	custom_textures(t_data *data, int txs, char **av, int ac)
{
	int bpp;
	int endian;
	int size_line;
	int i;

	data->tx_ptrs = malloc(sizeof(void *) * txs);
	data->tx_datas = malloc(sizeof(int *) * txs);
	i = 1;
	txs = -1;
	while (++i < ac)
	{
		txs++;
		if (!(data->tx_ptrs[txs] = mlx_xpm_file_to_image(data->mlx_ptr,
		av[i], &data->w[txs], &data->h[txs])))
		{
			ft_putstr("There is no such file: ");
			ft_putstr(av[i]);
			ft_putchar('\n');
			txs--;
			continue ;
		}
		data->tx_datas[txs] = (int*)mlx_get_data_addr(data->tx_ptrs[txs],
		&bpp, &size_line, &endian);
	}
}

void	crt_textures(char **av, int ac, t_data *data)
{
	int j;
	int i;

	i = 1;
	j = 0;
	while (++i < ac)
		if (mlx_xpm_file_to_image(data->mlx_ptr,
		av[i], &data->w[j], &data->h[j]))
			j++;
	data->textures = j;
	if (j > 0)
	{
		custom_textures(data, j, av, ac);
	}
	else
		compass_facing(data);
}

void	crt_floor_and_ceiling_tex(t_data *data)
{
	int bpp;
	int endian;
	int size_line;

	if (!(data->floor_tex = mlx_xpm_file_to_image(data->mlx_ptr,
	"txs/floor.xpm", &data->floor_w, &data->floor_h)))
	{
		ft_putstr("There is no floor texture =(.\n");
		exit(42);
	}
	if (!(data->ceiling_tex = mlx_xpm_file_to_image(data->mlx_ptr,
	"txs/metalic_ceiling.xpm", &data->ceiling_w, &data->ceiling_h)))
	{
		ft_putstr("There is no ceiling texture =(.\n");
		exit(42);
	}
	data->floor_data = (int*)mlx_get_data_addr(data->floor_tex,
	&bpp, &size_line, &endian);
	data->ceiling_data = (int*)mlx_get_data_addr(data->ceiling_tex,
	&bpp, &size_line, &endian);
}
