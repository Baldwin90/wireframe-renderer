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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define DA data->alpha
#define DAP data_addpoint
#define DB data->beta
#define DD data->draw_stats
#define DG data->gamma
#define DI data->increment
#define DW data->window
#define FS FREE_SPLITS(fields, field_count)
#define FSFEIII FS; fdf_error(3, line, fd, data)
#define FSFEIV FS; fdf_error(4, line, fd, data)
#define FSFEV FS; fdf_error(5, line, fd, data)
#define FSRE FREE_SPLITS(point, point_data_count);return (FALSE);
#define MGDA mlx_get_data_addr
#define RV rgb_val
#define C line = NULL; gnl_res = get_next_line(fd, &line); z = 0;
#define A int gnl_res; char *line;char **fields;int field_count; int z; C;
#define B char **point;int point_data_count;int RV; float hsbvals[3]; int x = 0;

int		key_hook(int key_code, t_mapdata *data)
{
	if (key_code == 53)
	{
		data_free(data);
		exit(1);
	}
	if (key_code == 126)
		data_set_value(data, &(DA), DA + DI);
	if (key_code == 125)
		data_set_value(data, &(DA), DA - DI);
	if (key_code == 123)
		data_set_value(data, &(DB), DB + DI);
	if (key_code == 124)
		data_set_value(data, &(DB), DB - DI);
	if (key_code == 12)
		data_set_value(data, &(DG), DG - DI);
	if (key_code == 14)
		data_set_value(data, &(DG), DG + DI);
	if (key_code == 15)
	{
		DI = 4;
		data_set_value(data, &(DA), 135);
		data_set_value(data, &(DB), 25);
		data_set_value(data, &(DG), 0);
	}
	if (key_code == 4)
	{
		DD = !DD;
		draw_image(data);
	}
	if (key_code == 7)
	{
		data->display_interlace = !data->display_interlace;
		draw_fdf(data);
	}
	if (key_code == 8)
	{
		data->anti_alias = !data->anti_alias;
		draw_fdf(data);
	}
	if (key_code == 33)
	{
		if (DI > 15)
			DI -= 5;
		else if (DI > 1)
			DI -= 1;
		else
			DI -= 0.1;
		if (DI < 0.1)
			DI = 0.1;
		draw_image(data);
	}
	if (key_code == 30)
	{
		if (DI >= 15)
			DI += 5;
		else if (DI >= 1)
			DI += 1;
		else
			DI += 0.1;
		if (DI > 45)
			DI = 45;
		draw_image(data);
	}
	return (0);
}

void	create_mlx(t_mapdata *data)
{
	DW->mlx = mlx_init();
	DW->win = mlx_new_window(DW->mlx, SCREENSIZE, SCREENSIZE, "FDF");
	DW->img = mlx_new_image(DW->mlx, SCREENSIZE, SCREENSIZE);
	DW->pixel = MGDA(DW->img, &(DW->bpp), &(DW->line_size), &(DW->endian));
	DW->bpp /= 8;
	mlx_expose_hook(DW->win, &draw_image, data);
	mlx_key_hook(DW->win, &key_hook, data);
}

int		create_point(t_mapdata *data, char **fields, int field_count, int z)
{
	B;
	while (x < field_count)
	{
		point = ft_strsplit(fields[x], ',');
		ARR_SIZE(point, point_data_count);
		if (point_data_count == 1)
			DAP(data, (float[]){x, ft_atoi(point[0]), z}, (float[]){0, 0, 1});
		else if (point_data_count == 2)
		{
			if ((RV = ft_htoi(point[1])) == -1 || RV > RGB_MAX)
			{
				FSRE;
			}
			rgb2hsb(RV >> 16, (RV & 0xFF00) >> 8, RV & 0xFF, hsbvals);
			DAP(data, (float[]){x, ft_atoi(point[0]), z}, hsbvals);
		}
		else
		{
			FSRE;
		}
		FREE_SPLITS(point, point_data_count);
		x += 1;
	}
	return (TRUE);
}

void	map_fill(t_mapdata *data, int fd)
{
	A;
	while (gnl_res == 1)
	{
		fields = ft_strsplit(line, ' ');
		ARR_SIZE(fields, field_count);
		if (data->x_size < 0)
			data->x_size = field_count;
		else if (field_count != data->x_size)
		{
			FSFEIII;
		}
		else if (field_count == 0)
		{
			FSFEIV;
		}
		if (!create_point(data, fields, field_count, z))
		{
			FSFEV;
		}
		FS;
		z += 1;
		gnl_res = get_next_line(fd, &line);
	}
	if (line != NULL)
		free(line);
}

void	fdf_error(int fdf_err_num, char *line, int fd, t_mapdata *data)
{
	if (line)
	{
		free(line);
	}
	if (fd != -1)
	{
		if (close(fd) == -1)
		{
			fdf_error(fdf_err_num + 1000, NULL, -1, data);
		}
	}
	if (data != NULL)
	{
		data_free(data);
	}
	exit(fdf_err_num);
}

int		main(int argc, char const *argv[])
{
	int			fd;
	t_mapdata	*data;

	if (argc != 2)
		fdf_error(1, NULL, -1, NULL);
	if ((fd = open(argv[1], O_RDONLY)) == -1)
		fdf_error(2, NULL, -1, NULL);
	data = data_create(0x141116);
	map_fill(data, fd);
	if (close(fd) == -1)
		fdf_error(1000, NULL, -1, data);
	create_mlx(data);
	draw_fdf(data);
	mlx_loop(DW->mlx);
	return (0);
}
