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

#define D data, &
#define DA data->alpha
#define DAP data_addpoint
#define DB data->beta
#define DD data->draw_stats
#define DDI data->display_interlace
#define DG data->gamma
#define DI data->increment
#define DW data->window
#define DSV data_set_value
#define KCE key_code ==
#define KC53 if (KCE 53) {data_free(data);exit(1);};
#define KC126 if (KCE 126) DSV(data, &(DA), DA + DI);
#define KC125 if (KCE 125) DSV(data, &(DA), DA - DI);
#define KC123 if (KCE 123) DSV(data, &(DB), DB + DI);
#define KC124 if (KCE 124) DSV(data, &(DB), DB - DI);
#define KC12 if (KCE 12) DSV(data, &(DG), DG - DI);
#define KC14 if (KCE 14) DSV(data, &(DG), DG + DI);
#define KC15 if (KCE 15) {DI = 4;DSV(D(DA), 135);DSV(D(DB), 25);DSV(D(DG), 0);};
#define KC4 if (KCE 4) {DD = !DD;draw_image(data);};
#define KC7 if (KCE 7) {DDI = !DDI;draw_fdf(data);};
#define KC8 if (KCE 8){data->anti_alias = !data->anti_alias;draw_fdf(data);};
#define KCEX KC53 KC126 KC125 KC123 KC124 KC12 KC14 KC15 KC7 KC8
#define IFDI01 if (DI < 0.1) DI = 0.1;
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
	KCEX;
	if (key_code == 33)
	{
		if (DI > 15)
			DI -= 5;
		else if (DI > 1)
			DI -= 1;
		else
			DI -= 0.1;
		IFDI01;
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
