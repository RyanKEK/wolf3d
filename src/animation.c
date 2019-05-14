/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohavryle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 12:47:20 by ohavryle          #+#    #+#             */
/*   Updated: 2019/04/08 12:47:21 by ohavryle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/wolf3d.h"
#define BPP &bpp, &size_line, &endian

void	crt_animation(t_data *data)
{
	int		i;
	int		bpp;
	int		endian;
	int		size_line;
	char	*frame;

	frame = (char*)malloc(sizeof(char) * 13);
	ft_memcpy(frame, "doge/frame1\0\0", 13);
	i = -1;
	data->an_ptrs = malloc(sizeof(void*) * 16);
	data->an_datas = malloc(sizeof(char*) * 16);
	while (++i < 16)
	{
		if (i >= 9)
		{
			frame[10] = (i + 1) / 10 + 48;
			frame[11] = (i + 1) % 10 + 48;
		}
		else
			frame[10] = i + 49;
		data->an_ptrs[i] = mlx_xpm_file_to_image(data->mlx_ptr,
		frame, &data->an_w[i], &data->an_h[i]);
		data->an_datas[i] = mlx_get_data_addr(data->an_ptrs[i], BPP);
	}
	free(frame);
}

void	time_spent(int *frame)
{
	static clock_t	previous;
	static int		c = 0;

	if (c == 0)
	{
		c++;
		previous = clock();
	}
	if ((clock() - previous) * 1000 / CLOCKS_PER_SEC >= 30)
	{
		(*frame)++;
		c = 0;
	}
	if (*frame > 15)
	{
		c = 0;
		*frame = 0;
	}
}

void	animation_tex(int start, int end, int tx, t_data *data)
{
	float		ty;
	int			index;
	int			tindex;
	int			y;
	static int	frame = 0;

	time_spent(&frame);
	tx = (int)((float)tx / (float)data->cell * (float)data->an_w[frame]);
	y = end;
	ty = 0;
	while (y < start)
	{
		index = (data->x + y * WIDTH) * 4;
		tindex = (tx + (int)ty * data->an_w[frame]) * 4;
		if (index + 2 < ((WIDTH - 1) + (HEIGHT - 1) * WIDTH) * 4 && index > 0
		&& tindex < ((data->an_w[frame] - 1) + (int)(data->an_h[frame] - 1)
		* data->an_w[frame]) * 4 && tindex > 0)
		{
			data->img_data[index] = data->an_datas[frame][tindex];
			data->img_data[index + 1] = data->an_datas[frame][tindex + 1];
			data->img_data[index + 2] = data->an_datas[frame][tindex + 2];
		}
		ty += (float)data->an_h[frame] / abs(start - end);
		y++;
	}
}
