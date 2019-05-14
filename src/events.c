/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohavryle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 14:35:03 by ohavryle          #+#    #+#             */
/*   Updated: 2019/03/27 14:35:04 by ohavryle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/wolf3d.h"

int		motion_hook(int x, int y, void *param)
{
	t_data		*data;
	static int	tmpx = 0;
	static int	c = 0;
	static int	tmpy = 0;

	data = (t_data *)param;
	if (c == 0)
	{
		tmpy = y;
		tmpx = x;
	}
	data->pov += (x - tmpx) / 5.0;
	data->ppc -= (y - tmpy) * 2;
	if (data->ppc > HEIGHT)
		data->ppc = HEIGHT;
	if (data->ppc < -50)
		data->ppc = -50;
	tmpy = y;
	tmpx = x;
	c++;
	return (0);
}

void	movement(t_data *data, int keycode)
{
	if (keycode == 123)
	{
		data->px = (int)(data->speed * cos((data->pov - 90) * RAD)) + data->px;
		data->py = (int)(data->speed * sin((data->pov - 90) * RAD)) + data->py;
	}
	else if (keycode == 124)
	{
		data->px = (int)(data->speed * cos((data->pov + 90) * RAD)) + data->px;
		data->py = (int)(data->speed * sin((data->pov + 90) * RAD)) + data->py;
	}
	if (keycode == 1 || keycode == 125)
		data->speed = -data->speed;
	if (data->map[((int)(data->speed * 2
		* sin(data->pov * RAD) + data->py)) / data->cell][((int)(data->speed
		* 2 * cos(data->pov * RAD) + data->px)) / data->cell] != '0')
	{
		data->speed = abs(data->speed);
		return ;
	}
	data->px = (int)(data->speed * cos(data->pov * RAD)) + data->px;
	data->py = (int)(data->speed * sin(data->pov * RAD)) + data->py;
	data->speed = abs(data->speed);
}

int		key_hook(int keycode, void *param)
{
	t_data *data;

	data = (t_data *)param;
	if (keycode == 53)
		exit(42);
	else if (keycode == 15)
		data->fov = 66;
	else if (keycode == 257)
	{
		if (data->speed == data->cell / 6)
			data->speed = data->cell / 3;
		else
			data->speed = data->cell / 6;
	}
	else if (keycode == 0)
		data->pov -= 5;
	else if (keycode == 2)
		data->pov += 5;
	else if (keycode == 123 || keycode == 124
	|| keycode == 1 || keycode == 125 || keycode == 126 || keycode == 13)
		movement(data, keycode);
	else
		toggle_fog_and_night(keycode, data);
	return (0);
}

void	change_texture(t_data *data)
{
	double nx;
	double ny;

	nx = data->px;
	ny = data->py;
	while (data->map[(int)ny / data->cell][(int)nx / data->cell] == '0')
	{
		nx = 2 * cos(data->pov * RAD) + nx;
		ny = 2 * sin(data->pov * RAD) + ny;
	}
	if (data->map[(int)ny / data->cell][(int)nx
		/ data->cell] < data->textures + 48)
		data->map[(int)ny / data->cell][(int)nx / data->cell]++;
	else
		data->map[(int)ny / data->cell][(int)nx / data->cell] = '1';
}

int		mouse_hook(int button, int x, int y, void *param)
{
	t_data *data;

	data = (t_data *)param;
	x = 0;
	y = 0;
	if (button == 1 && data->textures > 0)
		change_texture(data);
	if (button == 4)
		data->fov++;
	else if (button == 5)
		data->fov--;
	if (data->fov < 40)
		data->fov = 40;
	else if (data->fov > 120)
		data->fov = 120;
	return (0);
}
