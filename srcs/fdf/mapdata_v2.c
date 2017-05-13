/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapdata.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achi <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/30 15:49:40 by achi              #+#    #+#             */
/*   Updated: 2017/01/30 15:49:42 by achi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <fdf.h>
#include <math.h>
#include <mlx.h>

t_mapdata		*data_create(int color)
{
	t_mapdata *data;

	data = ft_memalloc(sizeof(*data));
	data->x_size = -1;
	data->alpha = 135;
	data->beta = 25;
	data->gamma = 0;
	data->increment = 4;
	data->draw_stats = 1;
	data->anti_alias = 1;
	data->display_interlace = 1;
	data->arr = arraylist_new(10, 1.5);
	data->window = ft_memalloc(sizeof(*(data->window)));
	data->background = color;
	data_update_rotation(data);
	return (data);
}

void			window_free(t_windata *w)
{
	if (w->mlx)
	{
		mlx_destroy_image(w->mlx, w->img);
		mlx_destroy_window(w->mlx, w->win);
	}
	free(w);
}

void			rotate_data_into_view(t_mapdata *data)
{
	int	i;

	i = 0;
	while (i < data->arr->size)
	{
		rotate_point_into_view(data, data->arr->data[i]);
		i++;
	}
}
