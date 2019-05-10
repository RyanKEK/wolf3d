/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   primitives.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohavryle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 14:56:26 by ohavryle          #+#    #+#             */
/*   Updated: 2019/03/27 14:56:27 by ohavryle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void    pixel_put(int x, int y, t_data data)
{
	int index;
	index = (x + y * WIDTH) * 4;
	if (index < ((WIDTH - 1) + (HEIGHT - 1) * WIDTH) * 4 && index > 0)
	{
		data.img_data[index] = mlx_get_color_value(data.mlx_ptr, data.color.b);
		data.img_data[index + 1] = mlx_get_color_value(data.mlx_ptr, data.color.g);
		data.img_data[index + 2] = mlx_get_color_value(data.mlx_ptr, data.color.r);
	}
}

void	put_line(t_point start, t_point end, t_data data)
{
	t_point	dis;
	int		stepx;
	int		stepy;
	int		err;
	int		e2;

	dis.x = abs(end.x - start.x);
	stepx = start.x < end.x ? 1 : -1;
	dis.y = -abs(end.y - start.y);
	stepy = start.y < end.y ? 1 : -1;
	err = dis.x + dis.y;
	while (1)
	{
		if (start.y < HEIGHT && start.x < WIDTH && start.y > 0 && start.x > 0)
			pixel_put(start.x, start.y, data);
		if (start.x == end.x && start.y == end.y)
			break ;
		e2 = 2 * err;
		if (e2 >= dis.y)
		{
			err += dis.y;
			start.x += stepx;
		}
		if (e2 <= dis.x)
		{
			err += dis.x;
			start.y += stepy;
		}
	}
}

void    rect(t_point point, int width, int height, t_data data)
{
	t_point tmp;

	tmp = point;
	tmp.x = point.x + width;
	put_line(point, tmp, data);
	point = tmp;
	tmp.y = point.y + height;
	put_line(point, tmp, data);
	point = tmp;
	tmp.x -= width;
	put_line(point, tmp, data);
	point = tmp;
	tmp.y -= height;
	put_line(point, tmp, data);
}

void    fill_rect(t_point point, int width, int height, t_data data)
{
	t_point fill_start;
	t_point fill_end;

	rect(point, width, height, data);
	fill_start.x = point.x + 1;
	fill_end.x = point.x + width - 1;
	fill_start.y = point.y;
	fill_end.y = point.y;
	while (fill_start.y <= point.y + height)
	{
		put_line(fill_start, fill_end, data);
		fill_start.y++;
		fill_end.y++;
	}
}
