/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_casting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohavryle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/07 15:03:14 by ohavryle          #+#    #+#             */
/*   Updated: 2019/04/07 15:03:16 by ohavryle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	wall_render(t_fpoint h, t_fpoint v, t_data *data)
{
	double h_intersec;
	double v_intersec;
	double height;
	t_point start;
	t_point end;

	h_intersec = sqrt((data->px - h.x) * (data->px - h.x) + (data->py - h.y) * (data->py - h.y));
	v_intersec = sqrt((data->px - v.x) * (data->px - v.x) + (data->py - v.y) * (data->py - v.y));
	if ((h_intersec >= v_intersec || (int)h_intersec < 0) && (int)v_intersec > 0)
	{
		height = (double)data->cell_size / v_intersec * ((WIDTH / 2.0) / tan((data->fov / 2.0) * RAD));
		if (data->textures > 0)
			data->tx_index = data->map[(int)v.y / data->cell_size][(int)v.x / data->cell_size] - 49;
		else
		{
			if (data->rounded_angle < 90 || data->rounded_angle > 270)
				data->tx_index = 1;
			else
				data->tx_index = 3;
		}
	}
	else{
		height = (double)data->cell_size / h_intersec * ((WIDTH / 2.0) / tan((data->fov / 2.0) * RAD));
		if (data->textures > 0)
			data->tx_index = data->map[(int)h.y / data->cell_size][(int)h.x / data->cell_size] - 49;
		else
		{
			if (data->rounded_angle > 180)
				data->tx_index = 0;
			else
				data->tx_index = 2;
		}
	}
	height = (height / cos(-data->right_angle * RAD));
	end.x = data->x;
	start.x = data->x;
	start.y = data->ppc + (int)height / 2;
	end.y = data->ppc - (int)height / 2;
	if ((h_intersec > v_intersec || (int)h_intersec < 0) && (int)v_intersec > 0)
	{
		if (data->map[(int)v.y / data->cell_size][(int)v.x / data->cell_size] == 'a')
			animation_texturing(start.y, end.y, (int)v.y % data->cell_size, data);
		else
			line_texturing(start.y, end.y, (int)v.y % data->cell_size, data);
		floor_casting(data, v_intersec, v, start.y);
		ceiling_casting(data, v_intersec, v, end.y);
	}
	else {
		if (data->map[(int)h.y / data->cell_size][(int)h.x / data->cell_size] == 'a')
			animation_texturing(start.y, end.y, (int)h.x % data->cell_size, data);
		else
			line_texturing(start.y, end.y, (int)h.x % data->cell_size, data);
		floor_casting(data, h_intersec, h, start.y);
		ceiling_casting(data, h_intersec, h, end.y);
	}
}

void    critical_angles(t_fpoint *horiz, t_fpoint *vert, t_data *data)
{
	if ((data->rounded_angle < 10 || data->rounded_angle > 350) && horiz->x < data->px)
	{
		horiz->x = vert->x + 1;
		horiz->y = vert->y + 1;
	}
	else if (data->rounded_angle > 170 && data->rounded_angle < 190 && horiz->x > data->px)
	{
		horiz->x = vert->x - 1;
		horiz->y = vert->y - 1;
	}
	else if (data->rounded_angle > 80 && data->rounded_angle < 100 && vert->y < data->py)
	{
		vert->x = horiz->x + 1;
		vert->y = horiz->y + 1;
	}
	else if (data->rounded_angle < 280 && data->rounded_angle > 260 && vert->y > data->py)
	{
		vert->x = horiz->x - 1;
		vert->y = horiz->y - 1;
	}
}

t_fpoint	check_and_proceed(t_fpoint current, t_fpoint next, t_data *data)
{
	while (1)
	{
		if (current.y / data->cell_size < 0 || current.y / data->cell_size >= data->map_rows
			|| current.x / data->cell_size < 0 || current.x / data->cell_size >= data->map_cols) {
			break ;
		}
		if (data->map[(int)(current.y / data->cell_size)][(int)(current.x / data->cell_size)] != '0')
			break ;
		current.x = current.x + next.x;
		current.y = current.y + next.y;
	}
	return (current);
}

void	first_inter(t_fpoint *inter, t_fpoint *next, t_data *data, char side)
{
	if (side == 'h')
	{
		if (data->rounded_angle > 180)
			inter->y = (int)(data->py / data->cell_size) * data->cell_size - 1;
		else
			inter->y = (int)(data->py / data->cell_size) * data->cell_size + data->cell_size;
		inter->x = data->px + (data->py - inter->y) / tan(-data->angle * RAD);
		if (data->rounded_angle > 180)
			next->y = -data->cell_size;
		else
			next->y = data->cell_size;
		next->x = next->y / tan(data->angle * RAD);
	}
	else
	{
		if (data->rounded_angle < 90 || data->rounded_angle > 270)
			inter->x = (int) (data->px / data->cell_size) * data->cell_size + data->cell_size;
		else
			inter->x = (int) (data->px / data->cell_size) * data->cell_size - 1;
		inter->y = data->py + (data->px - inter->x) * tan(-data->angle * RAD);
		if (data->rounded_angle < 90 || data->rounded_angle > 270)
			next->x = data->cell_size;
		else
			next->x = -data->cell_size;
		next->y = next->x * tan(data->angle * RAD);
	}
}

void	ray_casting(t_data *data)
{
	t_fpoint	v_nearest;
	t_fpoint	h_nearest;
	t_fpoint	next;
	data->right_angle = -(data->fov / 2);
	data->x = -1;
	data->angle = data->pov - (data->fov / 2);
	while (++data->x < WIDTH)
	{
		data->rounded_angle = (360 + ((int)data->angle % 360)) % 360;
		first_inter(&h_nearest, &next, data, 'h');
		h_nearest = check_and_proceed(h_nearest, next, data);
		first_inter(&v_nearest, &next, data, 'v');
		v_nearest = check_and_proceed(v_nearest, next, data);
		critical_angles(&h_nearest, &v_nearest, data);
		wall_render(h_nearest, v_nearest, data);
		data->angle += data->fov / WIDTH;
		data->right_angle += data->fov / WIDTH;
	}
}
