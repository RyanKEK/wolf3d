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

#include "../includes/wolf3d.h"
#define STEPX {err += dis.y; start.x += stepx;}
#define STEPY {err += dis.x; start.y += stepy;}

void	pixel_put(int x, int y, t_data data)
{
	int index;

	index = (x + y * WIDTH) * 4;
	if (index < ((WIDTH - 1) + (HEIGHT - 1) * WIDTH) * 4 && index > 0)
	{
		data.img_data[index] = mlx_get_color_value(data.mlx_ptr,
		data.color.b);
		data.img_data[index + 1] = mlx_get_color_value(data.mlx_ptr,
		data.color.g);
		data.img_data[index + 2] = mlx_get_color_value(data.mlx_ptr,
		data.color.r);
	}
}

void	set_color(t_data *data, int r, int g, int b)
{
	data->color.r = r;
	data->color.g = g;
	data->color.b = b;
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
			STEPX;
		if (e2 <= dis.x)
			STEPY;
	}
}

void	toggle_fog_and_night(int keycode, t_data *data)
{
	if (keycode == 37)
	{
		if (data->shadows)
			data->shadows--;
		else
		{
			data->fog = 0;
			data->shadows++;
		}
	}
	else if (keycode == 3)
	{
		if (data->fog)
			data->fog--;
		else
		{
			data->shadows = 0;
			data->fog++;
		}
	}
}

void	crosshair(t_data *data)
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
