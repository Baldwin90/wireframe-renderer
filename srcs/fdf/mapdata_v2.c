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

#define F0000 (float[]){0, 0, 0, 0}
#define F1000 (float[]){1, 0, 0, 0}
#define F0100 (float[]){0, 1, 0, 0}
#define F0010 (float[]){0, 0, 1, 0}
#define F0001 (float[]){0, 0, 0, 1}
#define F0SACA0 (float[]){0, sin(alpha), cos(alpha), 0}
#define FCB0SB0 (float[]){cos(beta), 0, sin(beta), 0}
#define FSGCG00 (float[]){sin(gamma), cos(gamma), 0, 0}
#define FLOATS float alpha;float beta;float gamma;
#define TMATRICES t_matrix *x; t_matrix *y;t_matrix *z;t_matrix *cache;

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

void			data_free(t_mapdata *data)
{
	int	i;

	i = 0;
	while (i < data->arr->size)
	{
		matrix_free(((t_point *)data->arr->data[i])->matrix_3d);
		free(data->arr->data[i]);
		i++;
	}
	arraylist_free(data->arr);
	matrix_free(data->rot_matrix);
	window_free(data->window);
	free(data);
}

void			data_update_rotation(t_mapdata *data)
{
	FLOATS;
	TMATRICES;
	alpha = (data->alpha) * 0.01745329252;
	beta = (data->beta) * 0.01745329252;
	gamma = (data->gamma) * 0.01745329252;
	x = matrix_create(4, 4, F1000, \
		(float[]){0, cos(alpha), -sin(alpha), 0}, F0SACA0, F0001);
	y = matrix_create(4, 4, FCB0SB0, F0100, \
		(float[]){-sin(beta), 0, cos(beta), 0}, F0001);
	z = matrix_create(4, 4, \
		(float[]){cos(gamma), -sin(gamma), 0, 0}, FSGCG00, F0010, F0001);
	cache = matrix_dotproduct(x, y);
	if (data->rot_matrix)
	{
		matrix_free(data->rot_matrix);
		data->rot_matrix = 0;
	}
	data->rot_matrix = matrix_dotproduct(cache, z);
	matrix_free(x);
	matrix_free(y);
	matrix_free(z);
	matrix_free(cache);
}

void			data_addpoint(t_mapdata *data, float coords[], float hsb[])
{
	t_point	*point;
	int		i;

	point = ft_memalloc(sizeof(*point));
	point->matrix_3d = matrix_create(1, 4, (float[]){coords[0]}, \
	(float[]){coords[1]}, (float[]){coords[2]}, (float[]){0});
	point->screen_x = -1;
	point->screen_y = -1;
	i = 0;
	while (i < 3)
	{
		point->hsb[i] = hsb[i];
		i++;
	}
	arraylist_add(data->arr, point);
	data_update_rotation(data);
	rotate_point_into_view(data, point);
}

void			data_set_value(t_mapdata *data, float *target, float value)
{
	while (value < 0)
	{
		value += 360;
	}
	while (value >= 360)
	{
		value -= 360;
	}
	*target = value;
	data_update_rotation(data);
	rotate_data_into_view(data);
	draw_fdf(data);
}

void			rotate_point_into_view(t_mapdata *data, t_point *point)
{
	t_matrix	*a;
	t_matrix	*b;
	t_matrix	*filter_matrix;

	filter_matrix = matrix_create(4, 4, F1000, F0100, F0000, F0001);
	a = matrix_dotproduct(data->rot_matrix, point->matrix_3d);
	b = matrix_dotproduct(filter_matrix, a);
	point->screen_x = b->data[0][0];
	point->screen_y = b->data[1][0];
	matrix_free(a);
	matrix_free(b);
	matrix_free(filter_matrix);
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
