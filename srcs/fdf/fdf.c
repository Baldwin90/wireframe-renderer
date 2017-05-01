/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achi <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/30 15:49:40 by achi              #+#    #+#             */
/*   Updated: 2017/01/30 15:49:42 by achi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <fdf.h>
#include <mlx.h>
#include <limits.h>

int key_hook(int key_code, t_mapdata *data)
{
	if (key_code == 53) {
		data_free(data);
		exit(1);
	}
	if (key_code == 126)
		data_set_value(data, &(data->alpha), data->alpha + data->increment);
	if (key_code == 125)
		data_set_value(data, &(data->alpha), data->alpha - data->increment);
	if (key_code == 123)
		data_set_value(data, &(data->beta), data->beta + data->increment);
	if (key_code == 124)
		data_set_value(data, &(data->beta), data->beta - data->increment);
	if (key_code == 12)
		data_set_value(data, &(data->gamma), data->gamma - data->increment);
	if (key_code == 14)
		data_set_value(data, &(data->gamma), data->gamma + data->increment);
	if (key_code == 15) {
		data->increment = 4;
		data_set_value(data, &(data->alpha), 135);
		data_set_value(data, &(data->beta), 25);
		data_set_value(data, &(data->gamma), 0);
	}
	if (key_code == 4) {
		data->draw_stats = !data->draw_stats;
		draw_image(data);
	}
	if (key_code == 7) {
		data->display_interlace = !data->display_interlace;
		draw_fdf(data);
	}
	if (key_code == 8) {
		data->anti_alias = !data->anti_alias;
		draw_fdf(data);
	}
	if (key_code == 33) {
		if (data->increment > 15) {
			data->increment -= 5;
		}
		else if (data->increment > 1) {
			data->increment -= 1;
		} else {
			data->increment -= 0.1;
		}
		if (data->increment < 0.1) {
			data->increment = 0.1;
		}
		draw_image(data);
	}
	if (key_code == 30) {
		if (data->increment >= 15) {
			data->increment += 5;
		}
		else if (data->increment >= 1) {
			data->increment += 1;
		} else {
			data->increment += 0.1;
		}
		if (data->increment > 45) {
			data->increment = 45;
		}
		draw_image(data);
	}
	return (0);
}

void	create_mlx(t_mapdata *data)
{
	data->window->mlx = mlx_init();
	data->window->win = mlx_new_window(data->window->mlx, SCREENSIZE, SCREENSIZE, "FDF");
	data->window->img = mlx_new_image(data->window->mlx, SCREENSIZE, SCREENSIZE);
	data->window->pixel = mlx_get_data_addr(data->window->img, &(data->window->bpp), &(data->window->line_size), &(data->window->endian));
	data->window->bpp /= 8;
	mlx_expose_hook(data->window->win, &draw_image, data);
	mlx_key_hook(data->window->win, &key_hook, data);
}

//YOUR JOB :3
void map_fill(t_mapdata *data)
{
	data->x_size = 19;
	int arr[209] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,0,0,10,10,0,0,0,10,10,10,10,10,0,0,0,0,0,10,10,0,0,10,10,0,0,0,0,0,0,0,10,10,0,0,0,0,10,10,0,0,10,10,0,0,0,0,0,0,0,10,10,0,0,0,0,10,10,10,10,10,10,0,0,0,0,10,10,10,10,0,0,0,0,0,0,10,10,10,10,10,0,0,0,10,10,0,0,0,0,0,0,0,0,0,0,0,0,10,10,0,0,0,10,10,0,0,0,0,0,0,0,0,0,0,0,0,10,10,0,0,0,10,10,10,10,10,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	for (int i = 0; i < 209; i++) {
		data_addpoint(data, (float []){(int)(i % data->x_size), arr[i], (int)(i / data->x_size)}, (float []){(float)i / 209, 1, 1});
	}
}

int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;

	// t_matrix *x = matrix_create(3, 3, (float []){1, 2, 3}, (float []){4,5,6}, (float []){7,8,9});
	// t_matrix *y = matrix_create(3, 3, (float []){10, 11, 12}, (float []){13,14,15}, (float []){16,17,18});
	// t_matrix *z = matrix_dotproduct(x, y);
	// matrix_free(x);
	// matrix_free(y);
	// matrix_free(z);
	t_mapdata *data = data_create(0x141116);
	map_fill(data);

	create_mlx(data);

	draw_fdf(data);

	mlx_loop(data->window->mlx);
	return (0);
}