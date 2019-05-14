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

#include "../includes/wolf3d.h"

void		wall_render(t_fpoint h, t_fpoint v, t_data *data)
{
	t_point	start;
	t_point	end;
	t_line	l;

	data->hlen = sqrt((data->px - h.x) *
	(data->px - h.x) + (data->py - h.y) * (data->py - h.y));
	data->vlen = sqrt((data->px - v.x) *
	(data->px - v.x) + (data->py - v.y) * (data->py - v.y));
	define_h(data, v, h);
	data->height = (data->height / cos(-data->right_angle * RAD));
	end.x = data->x;
	start.x = data->x;
	start.y = data->ppc + (int)data->height / 2;
	end.y = data->ppc - (int)data->height / 2;
	l.start = start;
	l.end = end;
	nearest_wall(data, v, h, l);
}

t_fpoint	check_and_proceed(t_fpoint current, t_fpoint next, t_data *data)
{
	while (1)
	{
		if (current.y / data->cell < 0
		|| current.y / data->cell >= data->map_rows
		|| current.x / data->cell < 0
		|| current.x / data->cell >= data->map_cols)
			break ;
		if (data->map[(int)(current.y
		/ data->cell)][(int)(current.x / data->cell)] != '0')
			break ;
		current.x = current.x + next.x;
		current.y = current.y + next.y;
	}
	return (current);
}

void		v_side(t_fpoint *inter, t_fpoint *next, t_data *data)
{
	if (data->rounded_angle < 90 || data->rounded_angle > 270)
		inter->x = (int)(data->px / data->cell) * data->cell + data->cell;
	else
		inter->x = (int)(data->px / data->cell) * data->cell - 1;
	inter->y = data->py + (data->px - inter->x) * tan(-data->angle * RAD);
	if (data->rounded_angle < 90 || data->rounded_angle > 270)
		next->x = data->cell;
	else
		next->x = -data->cell;
	next->y = next->x * tan(data->angle * RAD);
}

void		first_inter(t_fpoint *inter,
t_fpoint *next, t_data *data, char side)
{
	if (side == 'h')
	{
		if (data->rounded_angle > 180)
			inter->y = (int)(data->py / data->cell) * data->cell - 1;
		else
			inter->y = (int)(data->py / data->cell) * data->cell + data->cell;
		inter->x = data->px + (data->py - inter->y) / tan(-data->angle * RAD);
		if (data->rounded_angle > 180)
			next->y = -data->cell;
		else
			next->y = data->cell;
		next->x = next->y / tan(data->angle * RAD);
	}
	else
		v_side(inter, next, data);
}

void		ray_casting(t_data *data)
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
