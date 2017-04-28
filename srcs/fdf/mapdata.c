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

void		data_update_rotation(t_mapdata *data)
{
	t_matrix *x;
	t_matrix *y;
	t_matrix *z;
	t_matrix *cache;
	double alpha;
	double beta;
	double gamma;

	alpha = (data->alpha) * 0.01745329252;
	beta = (data->beta) * 0.01745329252;
	gamma = (data->gamma) * 0.01745329252;

	x = matrix_create(3, 3, (double[]) {1, 0, 0}, (double[]){0, cos(alpha), sin(alpha)}, (double[]){0, -sin(alpha), cos(alpha)});
	y = matrix_create(3, 3, (double[]){cos(beta), 0, -sin(beta)}, (double[]){0, 1, 0}, (double[]){sin(beta), 0, cos(beta)});
	z = matrix_create(3, 3, (double[]){cos(gamma), sin(gamma), 0}, (double[]){-sin(gamma), cos(gamma), 0}, (double[]){0, 0, 1});

	cache = matrix_dotproduct(x, y);
	data->rot_matrix = matrix_dotproduct(cache, z);
	matrix_free(x);
	matrix_free(y);
	matrix_free(z);
	matrix_free(cache);
}

void		data_addpoint(t_mapdata *data, double coords[], float hsb[])
{
	t_point *point;
	point = ft_memalloc(sizeof(*point));
	point->matrix_3d = matrix_create(1, 3, (double[]){coords[0]}, (double[]){coords[1]}, (double[]){coords[2]});
	point->screen_x = -1;
	point->screen_y = -1;
	for (int i = 0; i < 3; i++) {
		point->hsb[i] = hsb[i];
	}
	arraylist_add(data->arr, point);
	data_update_rotation(data);
	rotate_point_into_view(data, point);
}

void			data_set_value(t_mapdata *data, double *target, double value)
{
	while (value < 0) {
		value += 360;
	}
	while (value >= 360) {
		value -= 360;
	}
	*target = value;
	data_update_rotation(data);
	rotate_data_into_view(data);
	draw_fdf(data);
}

void			rotate_point_into_view(t_mapdata *data, t_point *point)
{
	t_matrix *a;
	t_matrix *b;

	t_matrix *filter_matrix = matrix_create(3, 3, (double[]){1, 0, 0}, (double[]){0, 1, 0}, (double[]){0, 0, 0});
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
	for (int i = 0; i < data->arr->size; i++) {
		rotate_point_into_view(data, data->arr->data[i]);
	}
}
