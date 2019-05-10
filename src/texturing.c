/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texturing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohavryle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/09 15:08:53 by ohavryle          #+#    #+#             */
/*   Updated: 2019/05/09 15:08:55 by ohavryle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	apply_weather(t_data *data, int index, int color, double intensity)
{
	data->int_data[index] = (((int)((color >> 16) *
			intensity + data->fog_value) & 0xFF) << 16) +
					(((int)(((color >> 8) & 0x00FF) *
					intensity + data->fog_value) & 0xFF) << 8)
			+ (((int)((color & 0x0000FF) * intensity + data->fog_value))
			& 0xFF);
}

void	floor_color(t_data *data, double weight, t_fpoint wall, double y)
{
	int			color;
	t_fpoint	floor;
	double		intensity;
	int			index;
	int			tindex;

	floor.x = weight * (wall.x / data->cell_size)
			+ (1.0 - weight) * (data->px / data->cell_size);
	floor.y = weight * (wall.y / data->cell_size)
			+ (1.0 - weight) * (data->py / data->cell_size);
	floor.x = (int)(floor.x * data->floor_w) % data->floor_w;
	floor.y = (int)(floor.y * data->floor_h) % data->floor_h;
	if (data->shadows || data->fog)
		intensity = (y - data->ppc) / (HEIGHT - data->ppc) * (1.0);
	else
		intensity = (1.0 - ((data->dist) /
				(HEIGHT / (2.0 * (data->bottom - 1 +
				((HEIGHT / 2.0) - data->ppc)) - HEIGHT)) * (0.5)));
	if (data->fog)
		data->fog_value = 255 * (1.0 - intensity);
	index = (data->x + (int)y * WIDTH);
	tindex = (int)(floor.x + floor.y * data->floor_w);
	if (tindex > (int)((data->floor_w - 1) + (data->floor_h - 1) * data->floor_w) || tindex < 0) {
		printf("floor.x:%f\nfloor.y:%f\nweight:%f\n", floor.x, floor.y, weight);
		return ;
	}
	color = (int)(data->floor_data[tindex]);
	if (index < (((WIDTH - 1) + (HEIGHT - 1) * WIDTH)) && index > 0)
		apply_weather(data, index, color, intensity);
}

void	ceiling_color(t_data *data, double weight, t_fpoint wall, double y)
{
	int			color;
	t_fpoint	ceiling;
	double		intensity;
	int			index;
	int			tindex;

	ceiling.x = weight * (wall.x / data->cell_size)
			+ (1.0 - weight) * (data->px / data->cell_size);
	ceiling.y = weight * (wall.y / data->cell_size)
			+ (1.0 - weight) * (data->py / data->cell_size);
	ceiling.x = (int)(ceiling.x * data->ceiling_w) % data->ceiling_w;
	ceiling.y = (int)(ceiling.y * data->ceiling_h) % data->ceiling_h;
	if (data->shadows || data->fog)
		intensity = 1.0 - (y / (data->ppc) * (1.0));
	else
		intensity = 1.0;
	if (data->fog)
		data->fog_value = 255 * (1.0 - intensity);
	index = (data->x + (int)y * WIDTH);
	tindex = (int)(ceiling.x + ceiling.y * data->ceiling_w);
	if (tindex > (int)((data->ceiling_w - 1) + (data->ceiling_h - 1) * data->ceiling_w) || tindex < 0) {
		printf("ceiling.x:%f\nceiling.y:%f\nweight:%f\n", ceiling.x, ceiling.y, weight);
		return ;
	}
	color = (int)(data->ceiling_data[tindex]);
	if (index < (((WIDTH - 1) + (HEIGHT - 1) * WIDTH)) && index > 0)
		apply_weather(data, index, color, intensity);
}

void	ceiling_casting(t_data *data, double wall_dist, t_fpoint wall, int top)
{
	double dist;
	double weight;
	double y;

	y = top + 1;
	data->fog_value = 0;
	while (--y >= 0)
	{
		dist = HEIGHT / (HEIGHT - 2.0 * (y + ((HEIGHT / 2.0) - data->ppc)));
		dist /= cos(data->right_angle * RAD);
		weight = (dist - 0.0) / ((wall_dist / data->cell_size) - 0.0);
		ceiling_color(data, weight, wall, y);
	}
}

void	floor_casting(t_data *data, double wall_dist, t_fpoint wall, int bottom)
{
	double dist;
	double weight;
	double y;

	y = bottom - 2;
	data->fog_value = 0;
	data->bottom = bottom;
	while (++y < HEIGHT)
	{
		dist = HEIGHT / (2 * (y + ((HEIGHT / 2) - data->ppc)) - HEIGHT);
		dist /= cos(data->right_angle * RAD);
		data->dist = dist;
		weight = dist / (wall_dist / data->cell_size);
		floor_color(data, weight, wall, y);
	}
}

void	line_texturing(int start, int end, double tx, t_data *data)
{
	double	ty;
	int		index;
	int		tindex;
	int		y;
	double	intensity;

	data->fog_value = 0;
	if (start - end < HEIGHT && (data->shadows || data->fog))
		intensity = ((double)(start - end) / (HEIGHT) * (100.0)) / 100.0;
	else
		intensity = 1;
	if (data->fog)
		data->fog_value = 255 * (1.0 - intensity);
	tx = tx / data->cell_size * data->w[data->tx_index];
	y = end;
	ty = 0;
	while (y < start && y < HEIGHT)
	{
		index = (data->x + y * WIDTH);
		tindex = ((int)tx + (int)ty * data->w[data->tx_index]);
		if (index < (((WIDTH - 1) + (HEIGHT - 1) * WIDTH)) && index > 0)
			apply_weather(data, index,
				(int)(data->tx_datas[data->tx_index][tindex]), intensity);
		ty += (double)data->h[data->tx_index] / abs(start - end);
		y++;
	}
}
