/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
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

#define DWW data->window->win
#define DWMLX data->window->mlx
#define DMLXW5 DWMLX, DWW, 5
#define DMLXW20 DWMLX, DWW, 20
#define HXF 0xFFFFFF
#define DYH DMLXW20, Y15, HXF
#define IE "Interlacing: ENABLED"
#define ID "Interlacing: DISABLED"
#define AAE "Anti-Aliasing: ENABLED"
#define AAD "Anti-Aliasing: DISABLED"
#define HIDDEN_STATS "Stats Hidden... Press 'H' to show."
#define Y15 y += 15
#define CFF cache[1] = ft_ftoa
#define SJ ft_strjoin
#define F float x_scale;float y_scale;float scale;float x_offset;float y_offset;
#define FXMINMAX float x_min=2147483647; float x_max=-2147483648;
#define FYMINMAX float y_min=2147483647; float y_max=-2147483648;
#define IIDXXTILE int idx = 0; int x_tiles; t_point *point;t_arraylist *arr
#define FXYSCALEOFFSETMINMAX F FXMINMAX FYMINMAX IIDXXTILE
#define DLFLOATS float hsbvals[3];float dx;float dy;float gradient;float y;
#define DLINTS int did_swap;int max_x;int start;int x;int color;
#define DLPARAMS char steep; DLFLOATS DLINTS float b_cache;

#define FREE_CACHE for(int i=0;i<10;i++){if(cache[i])free(cache[i]);cache[i]=0;}

char	*ft_ftoa(float f)
{
	char	buf[48];

	snprintf(buf, sizeof(buf), "%.2f", f);
	return (ft_strdup(buf));
}

void	draw_stats(t_mapdata *data)
{
	void	*cache[10];
	int		y;

	y = -15;
	if (data->draw_stats)
	{
		mlx_string_put(DMLXW5, Y15, HXF, "Stats");
		mlx_string_put(DMLXW5, Y15, HXF, "-------------------");
		mlx_string_put(DMLXW5, Y15, HXF, cache[0] = SJ("Vertex Count: ", cache[1] = ft_itoa(data->arr->size)));
		FREE_CACHE;
		mlx_string_put(DMLXW5, Y15, HXF, cache[0] = SJ("Line Count: ", cache[1] = ft_itoa( ((data->display_interlace) ? ((data->x_size - 1) * (data->arr->size / data->x_size - 1) * 4) + (data->x_size - 1) + (data->arr->size / data->x_size - 1) : ((data->x_size - 1) * (data->arr->size / data->x_size - 1) * 2) + (data->x_size - 1) + (data->arr->size / data->x_size - 1) ))));
		FREE_CACHE;
		mlx_string_put(DMLXW5, Y15, HXF, data->display_interlace ? IE : ID);
		mlx_string_put(DMLXW5, Y15, HXF, data->anti_alias ? AAE : AAD);
		mlx_string_put(DMLXW5, Y15, HXF, "Rotation (in degrees):");
		mlx_string_put(DYH, cache[0] = SJ("Increment: ", CFF(data->increment)));
		FREE_CACHE;
		mlx_string_put(DYH, cache[0] = SJ("Alpha: ", CFF(data->alpha)));
		FREE_CACHE;
		mlx_string_put(DYH, cache[0] = SJ("Beta: ", CFF(data->beta)));
		FREE_CACHE;
		mlx_string_put(DYH, cache[0] = SJ("Gamma: ", CFF(data->gamma)));
		FREE_CACHE;
		mlx_string_put(DMLXW5, y += 35, HXF, "Key Bindings");
		mlx_string_put(DMLXW5, Y15, HXF, "-------------------");
		mlx_string_put(DMLXW5, Y15, HXF, "Increment++: ']'");
		mlx_string_put(DMLXW5, Y15, HXF, "Increment--: '['");
		mlx_string_put(DMLXW5, Y15, HXF, "Alpha++: Up Arrow");
		mlx_string_put(DMLXW5, Y15, HXF, "Alpha--: Down Arrow");
		mlx_string_put(DMLXW5, Y15, HXF, "Beta++: Left Arrow");
		mlx_string_put(DMLXW5, Y15, HXF, "Beta--: Right Arrow");
		mlx_string_put(DMLXW5, Y15, HXF, "Gamma++: 'E'");
		mlx_string_put(DMLXW5, Y15, HXF, "Gamma--: 'Q'");
		mlx_string_put(DMLXW5, Y15, HXF, "Reset Orientation: 'R'");
		mlx_string_put(DMLXW5, Y15, HXF, "Toggle Anti-Aliasing: 'C'");
		mlx_string_put(DMLXW5, Y15, HXF, "Toggle Interlacing: 'X'");
		mlx_string_put(DMLXW5, Y15, HXF, "Toggle Stats: 'H'");
		mlx_string_put(DMLXW5, Y15, HXF, "Quit: 'esc'");
	}
	else
		mlx_string_put(DMLXW5, Y15, 0x515151, HIDDEN_STATS);
}

int		draw_image(t_mapdata *data)
{
	mlx_clear_window(DWMLX, DWW);
	mlx_put_image_to_window(DWMLX, DWW, data->window->img, 0, 0);
	draw_stats(data);
	return (0);
}

void	draw_pixel(t_windata *data, int x, int y, int color)
{
	if (x < 0 || x >= SCREENSIZE || y < 0 || y >= SCREENSIZE)
		return ;
	*((int *)(data->pixel + ((x + y * SCREENSIZE) * data->bpp))) = color;
}

void	draw_line(t_mapdata *data, ABCOLOR)
{
	DLPARAMS;
	steep = ABS(b[1] - a[1]) > ABS(b[0] - a[0]);
	did_swap = 0;
	if (steep)
	{
		mem_swap(a, a + 1, sizeof(*a));
		mem_swap(b, b + 1, sizeof(*a));
	}
	if (a[0] > b[0])
	{
		mem_swap(a, b, 2 * sizeof(float));
		did_swap = 1;
	}
	dx = (b[0] - a[0]) * SCREENSIZE;
	dy = ABS((b[1] - a[1])) * SCREENSIZE;
	gradient = dy / dx * (a[1] < b[1] ? 1 : -1);
	y = (a[1] * SCREENSIZE);
	max_x = (int)(b[0] * SCREENSIZE);
	start = (int)(a[0] * SCREENSIZE);
	x = start;
	while (x < max_x)
	{
		if (did_swap)
			hsb_lerp(color_b, color_a, (float)(x - start) / (float)(max_x - start - 1), &(hsbvals[0]));
		else
			hsb_lerp(color_a, color_b, (float)(x - start) / (float)(max_x - start - 1), &(hsbvals[0]));
		b_cache = hsbvals[2];
		if (data->anti_alias)
		{
			hsbvals[2] = lerp(0, b_cache, y - (int)y);
			color = hsb2rgb(hsbvals);
			if (steep)
				draw_pixel(data->window, y + 1, x, color);
			else
				draw_pixel(data->window, x, y + 1, color);
			hsbvals[2] = lerp(0, b_cache, 1 - (y - (int)y));
		}
		color = hsb2rgb(hsbvals);
		if (steep)
			draw_pixel(data->window, y, x, color);
		else
			draw_pixel(data->window, x, y, color);
		y += gradient;
		x++;
	}
}

void	draw_background(t_mapdata *data)
{
	int	i;

	i = 0;
	while (i < SCREENSIZE * SCREENSIZE)
	{
		((int *)data->window->pixel)[i] = data->background;
		i++;
	}
}

void	mem_swap(void *a, void *b, size_t size)
{
	char			cache;
	char			*a_mem;
	char			*b_mem;
	unsigned long	i;

	a_mem = a;
	b_mem = b;
	i = 0;
	while (i < size)
	{
		cache = a_mem[i];
		a_mem[i] = b_mem[i];
		b_mem[i] = cache;
		i++;
	}
}

void	draw_fdf(t_mapdata *data)
{
	FXYSCALEOFFSETMINMAX;
	arr = data->arr;
	while (idx < arr->size)
	{
		point = arr->data[idx];
		if (point->screen_x > x_max)
			x_max = point->screen_x;
		if (point->screen_x < x_min)
			x_min = point->screen_x;
		if (point->screen_y > y_max)
			y_max = point->screen_y;
		if (point->screen_y < y_min)
			y_min = point->screen_y;
		idx++;
	}
	x_scale = 1 / (x_max - x_min) * 0.95;
	y_scale = (1 / (y_max - y_min)) * 0.95;
	scale = MIN(x_scale, y_scale);
	x_offset = (1 - ((x_max - x_min) * scale)) * 0.5;
	y_offset = (1 - ((y_max - y_min) * scale)) * 0.5;
	draw_background(data);
	x_tiles = data->x_size;
	idx = 0;
	while (idx < arr->size - 1)
	{
		if (idx + x_tiles < arr->size)
		{
			draw_line(data, (float[]){((((t_point *)arr->data[idx])->screen_x - x_min) * scale) + x_offset, ((((t_point *)arr->data[idx])->screen_y - y_min) * scale) + y_offset}, (float[]){((((t_point *)arr->data[idx + x_tiles])->screen_x - x_min) * scale) + x_offset, ((((t_point *)arr->data[idx + x_tiles])->screen_y - y_min) * scale) + y_offset}, ((t_point *)arr->data[idx])->hsb, ((t_point *)arr->data[idx + x_tiles])->hsb);
		}
		if (idx % x_tiles != x_tiles - 1)
		{
			if (data->display_interlace && idx + x_tiles + 1 < arr->size)
			{
				draw_line(data, (float[]){((((t_point *)arr->data[idx])->screen_x - x_min) * scale) + x_offset, ((((t_point *)arr->data[idx])->screen_y - y_min) * scale) + y_offset}, (float[]){((((t_point *)arr->data[idx + x_tiles + 1])->screen_x - x_min) * scale) + x_offset, ((((t_point *)arr->data[idx + x_tiles + 1])->screen_y - y_min) * scale) + y_offset}, ((t_point *)arr->data[idx])->hsb, ((t_point *)arr->data[idx + x_tiles + 1])->hsb);
			}
			draw_line(data, (float[]){((((t_point *)arr->data[idx])->screen_x - x_min) * scale) + x_offset, ((((t_point *)arr->data[idx])->screen_y - y_min) * scale) + y_offset}, (float[]){((((t_point *)arr->data[idx + 1])->screen_x - x_min) * scale) + x_offset, ((((t_point *)arr->data[idx + 1])->screen_y - y_min) * scale) + y_offset}, ((t_point *)arr->data[idx])->hsb, ((t_point *)arr->data[idx + 1])->hsb);
		}
		if (idx % x_tiles != 0)
		{
			if (data->display_interlace && idx + x_tiles - 1 < arr->size)
			{
				draw_line(data, (float[]){((((t_point *)arr->data[idx])->screen_x - x_min) * scale) + x_offset, ((((t_point *)arr->data[idx])->screen_y - y_min) * scale) + y_offset}, (float[]){((((t_point *)arr->data[idx + x_tiles - 1])->screen_x - x_min) * scale) + x_offset, ((((t_point *)arr->data[idx + x_tiles - 1])->screen_y - y_min) * scale) + y_offset}, ((t_point *)arr->data[idx])->hsb, ((t_point *)arr->data[idx + x_tiles - 1])->hsb);
			}
		}
		idx++;
	}
	draw_image(data);
}
