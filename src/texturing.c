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

#include "../includes/wolf3d.h"
#define PLS (int)(data->tx_datas[data->tx_index][tindex])
#define MAPH (2.0 * (data->bottom - 1 + ((HEIGHT / 2.0) - data->ppc)) - HEIGHT)
#define MAXFTX (data->floor_w - 1) + (data->floor_h - 1) * data->floor_w
#define MAXCTX (data->ceiling_w - 1) + (data->ceiling_h - 1) * data->ceiling_w

void	floor_color(t_data *data, double weight, t_fpoint wall, double y)
{
	int			color;
	t_fpoint	floor;
	double		intensity;
	int			index;
	int			tindex;

	floor.x = weight * (wall.x / data->cell)
			+ (1.0 - weight) * (data->px / data->cell);
	floor.y = weight * (wall.y / data->cell)
			+ (1.0 - weight) * (data->py / data->cell);
	floor.x = (int)(floor.x * data->floor_w) % data->floor_w;
	floor.y = (int)(floor.y * data->floor_h) % data->floor_h;
	if (data->shadows || data->fog)
		intensity = (y - data->ppc) / (HEIGHT - data->ppc) * (1.0);
	else
		intensity = (1.0 - ((data->dist) / (HEIGHT / MAPH) * (0.5)));
	if (data->fog)
		data->fog_value = 255 * (1.0 - intensity);
	index = (data->x + (int)y * WIDTH);
	tindex = (int)(floor.x + floor.y * data->floor_w);
	if (tindex > (int)MAXFTX || tindex < 0)
		return ;
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

	ceiling.x = weight * (wall.x / data->cell)
			+ (1.0 - weight) * (data->px / data->cell);
	ceiling.y = weight * (wall.y / data->cell)
			+ (1.0 - weight) * (data->py / data->cell);
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
	if (tindex > (int)MAXCTX || tindex < 0)
		return ;
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
		weight = (dist - 0.0) / ((wall_dist / data->cell) - 0.0);
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
		weight = dist / (wall_dist / data->cell);
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
	tx = tx / data->cell * data->w[data->tx_index];
	y = end;
	ty = 0;
	while (y < start && y < HEIGHT)
	{
		index = (data->x + y * WIDTH);
		tindex = ((int)tx + (int)ty * data->w[data->tx_index]);
		if (index < (((WIDTH - 1) + (HEIGHT - 1) * WIDTH)) && index > 0)
			apply_weather(data, index, PLS, intensity);
		ty += (double)data->h[data->tx_index] / abs(start - end);
		y++;
	}
}
