/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_casting2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohavryle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 18:36:53 by ohavryle          #+#    #+#             */
/*   Updated: 2019/05/14 18:36:55 by ohavryle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/wolf3d.h"
#define A (double)data->cell / data->vlen
#define H data->map[(int)v.y / data->cell][(int)v.x / data->cell]
#define U (double)data->cell / data->hlen * ((WIDTH / 2.0)
#define E data->map[(int)h.y / data->cell][(int)h.x / data->cell]

void	compass_index(t_data *data, char side)
{
	if (side == 'v')
	{
		if (data->rounded_angle < 90 || data->rounded_angle > 270)
			data->tx_index = 1;
		else
			data->tx_index = 3;
	}
	else
	{
		if (data->rounded_angle > 180)
			data->tx_index = 0;
		else
			data->tx_index = 2;
	}
}

void	define_h(t_data *data, t_fpoint v, t_fpoint h)
{
	if ((data->hlen > data->vlen || (int)data->hlen < 0) && (int)data->vlen > 0)
	{
		data->height = (double)data->cell / data->vlen
		* ((WIDTH / 2.0) / tan((data->fov / 2.0) * RAD));
		if (data->textures > 0)
			data->tx_index = data->map[(int)v.y
			/ data->cell][(int)v.x / data->cell] - 49;
		else
			compass_index(data, 'v');
	}
	else
	{
		data->height = (double)data->cell / data->hlen
		* ((WIDTH / 2.0) / tan((data->fov / 2.0) * RAD));
		if (data->textures > 0)
			data->tx_index = data->map[(int)h.y
			/ data->cell][(int)h.x / data->cell] - 49;
		else
			compass_index(data, 'h');
	}
}

void	nearest_wall(t_data *data, t_fpoint v, t_fpoint h, t_line l)
{
	if ((data->hlen > data->vlen || (int)data->hlen < 0) && (int)data->vlen > 0)
	{
		if (data->map[(int)v.y / data->cell][(int)v.x / data->cell] == 'a')
			animation_tex(l.start.y, l.end.y, (int)v.y % data->cell, data);
		else
			line_texturing(l.start.y, l.end.y, (int)v.y % data->cell, data);
		floor_casting(data, data->vlen, v, l.start.y);
		ceiling_casting(data, data->vlen, v, l.end.y);
	}
	else
	{
		if (data->map[(int)h.y / data->cell][(int)h.x / data->cell] == 'a')
			animation_tex(l.start.y, l.end.y, (int)h.x % data->cell, data);
		else
			line_texturing(l.start.y, l.end.y, (int)h.x % data->cell, data);
		floor_casting(data, data->hlen, h, l.start.y);
		ceiling_casting(data, data->hlen, h, l.end.y);
	}
}

void	critical_angles(t_fpoint *horiz, t_fpoint *vert, t_data *data)
{
	if ((data->rounded_angle < 10
	|| data->rounded_angle > 350) && horiz->x < data->px)
	{
		horiz->x = vert->x + 1;
		horiz->y = vert->y + 1;
	}
	else if (data->rounded_angle > 170
	&& data->rounded_angle < 190 && horiz->x > data->px)
	{
		horiz->x = vert->x - 1;
		horiz->y = vert->y - 1;
	}
	else if (data->rounded_angle > 80
	&& data->rounded_angle < 100 && vert->y < data->py)
	{
		vert->x = horiz->x + 1;
		vert->y = horiz->y + 1;
	}
	else if (data->rounded_angle < 280
	&& data->rounded_angle > 260 && vert->y > data->py)
	{
		vert->x = horiz->x - 1;
		vert->y = horiz->y - 1;
	}
}

void	apply_weather(t_data *data, int index, int color, double intensity)
{
	data->int_data[index] = (((int)((color >> 16) *
			intensity + data->fog_value) & 0xFF) << 16) +
					(((int)(((color >> 8) & 0x00FF) *
					intensity + data->fog_value) & 0xFF) << 8)
			+ (((int)((color & 0x0000FF) * intensity + data->fog_value))
			& 0xFF);
}
