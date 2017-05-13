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

#define IE "Interlacing: ENABLED"
#define ID "Interlacing: DISABLED"
#define AAE "Anti-Aliasing: ENABLED"
#define AAD "Anti-Aliasing: DISABLED"
#define HIDDEN_STATS "Stats Hidden... Press 'H' to show."
#define T0 "Stats"
#define T1 "-------------------"
#define T2 "Vertex Count: "
#define T3 "Line Count: "
#define T4 "Rotation (in degrees):"
#define T5 "Increment: "
#define T6 "Alpha: "
#define T7 "Beta: "
#define T8 "Gamma: "
#define T9 "Key Bindings"
#define T10 "-------------------"
#define T11 "Increment++: ']'"
#define T12 "Increment--: '['"
#define T13 "Alpha++: Up Arrow"
#define T14 "Alpha--: Down Arrow"
#define T15 "Beta++: Left Arrow"
#define T16 "Beta--: Right Arrow"
#define T17 "Gamma++: 'E'"
#define T18 "Gamma--: 'Q'"
#define T19 "Reset Orientation: 'R'"
#define T20 "Toggle Anti-Aliasing: 'C'"
#define T21 "Toggle Interlacing: 'X'"
#define T22 "Toggle Stats: 'H'"
#define T23 "Quit: 'esc'"
#define DI data->increment
#define DA data->alpha
#define DAA data->anti_alias
#define DB data->beta
#define DG data->gamma
#define DX data->x_size
#define DAS data->arr->size
#define DDI data->display_interlace
#define DX1 (DX - 1)
#define DDX1 (DAS / DX - 1)
#define DWW data->window->win
#define DWMLX data->window->mlx
#define DMLXW5 DWMLX, DWW, 5
#define DMLXW20 DWMLX, DWW, 20
#define HXF 0xFFFFFF
#define D20YH DMLXW20, Y15, HXF
#define D5YH DMLXW5, Y15, HXF
#define Y15 y += 15
#define CFF cache[1] = ft_ftoa
#define SJ ft_strjoin
#define F float x_scale;float y_scale;float scale;float x_offset;float y_offset;
#define FXMINMAX float x_min=2147483647; float x_max=-2147483648;
#define FYMINMAX float y_min=2147483647; float y_max=-2147483648;
#define IDXXTILE int idx = -1; int x_tiles; t_point *point;t_arraylist *arr
#define PSX point->screen_x
#define PSY point->screen_y
#define PSXMM if (PSX > x_max) x_max = PSX;if (PSX < x_min) x_min = PSX;
#define PSYMM if (PSY > y_max) y_max = PSY;if (PSY < y_min) y_min = PSY;
#define WHPS while (++idx < arr->size) {point = arr->data[idx]; PSXMM; PSYMM;}
#define FXYSCALEOFFSETMINMAX F FXMINMAX FYMINMAX IDXXTILE; arr = data->arr; WHPS
#define DLFLOATS float hsb[3];float dx;float dy;float gradient;float y;
#define DLINTS int did_swap;int max_x;int start;int x;int color;
#define DLPARAMS char steep; DLFLOATS DLINTS float b_cache;
#define DSVAR void *cache[10];int y = -15;
#define CSJ cache[0] = SJ
#define MAT cache[1]=ft_itoa((DDI?(DX1*DDX1*4)+DX1+DDX1:(DX1*DDX1*2)+DX1+DDX1))
#define FREE_CACHE for(int i=0;i<10;i++){if(cache[i])free(cache[i]);cache[i]=0;}
#define FXS (float)(x - start)
#define MSP mlx_string_put
#define MSP1011 MSP(D5YH, T10);MSP(D5YH, T11);
#define MSP1215 MSP(D5YH, T12);MSP(D5YH, T13);MSP(D5YH, T14);MSP(D5YH, T15);
#define MSP1619 MSP(D5YH, T16);MSP(D5YH, T17);MSP(D5YH, T18);MSP(D5YH, T19);
#define MSP2023 MSP(D5YH, T20);MSP(D5YH, T21);MSP(D5YH, T22);MSP(D5YH, T23);
#define MSP10UP MSP1011 MSP1215 MSP1619 MSP2023
#define MSPFREE(code) mlx_string_put(code); FREE_CACHE
#define PNT(x) ((t_point *)arr->data[(x)])
#define PNT_X(idx) ((PNT(idx)->screen_x - x_min) * scale) + x_offset
#define PNT_Y(idx) ((PNT(idx)->screen_y - y_min) * scale) + y_offset
#define PNT_XY(idx) (float[]){PNT_X(idx), PNT_Y(idx)}

char	*ft_ftoa(float f)
{
	char	buf[48];

	snprintf(buf, sizeof(buf), "%.2f", f);
	return (ft_strdup(buf));
}

void	draw_stats(t_mapdata *data)
{
	DSVAR;
	if (data->draw_stats)
	{
		MSP(D5YH, T0);
		MSP(D5YH, T1);
		MSP(D5YH, CSJ(T2, cache[1] = ft_itoa(DAS)));
		FREE_CACHE;
		MSP(D5YH, CSJ(T3, MAT));
		FREE_CACHE;
		MSP(D5YH, DDI ? IE : ID);
		MSP(D5YH, DAA ? AAE : AAD);
		MSP(D5YH, T4);
		MSP(D20YH, CSJ(T5, CFF(DI)));
		FREE_CACHE;
		MSP(D20YH, CSJ(T6, CFF(DA)));
		FREE_CACHE;
		MSP(D20YH, CSJ(T7, CFF(DB)));
		FREE_CACHE;
		MSP(D20YH, CSJ(T8, CFF(DG)));
		MSP(DMLXW5, y += 35, HXF, T9);
		MSP10UP;
	}
	else
		MSP(DMLXW5, Y15, 0x515151, HIDDEN_STATS);
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

void	draw_line(t_mapdata *data, ABCOLOR cb[])
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
			hsb_lerp(cb, ca, FXS / (float)(max_x - start - 1), &(hsb[0]));
		else
			hsb_lerp(ca, cb, FXS / (float)(max_x - start - 1), &(hsb[0]));
		b_cache = hsb[2];
		if (data->anti_alias)
		{
			hsb[2] = lerp(0, b_cache, y - (int)y);
			color = hsb2rgb(hsb);
			if (steep)
				draw_pixel(data->window, y + 1, x, color);
			else
				draw_pixel(data->window, x, y + 1, color);
			hsb[2] = lerp(0, b_cache, 1 - (y - (int)y));
		}
		color = hsb2rgb(hsb);
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
	x_scale = 1 / (x_max - x_min) * 0.95;
	y_scale = (1 / (y_max - y_min)) * 0.95;
	scale = MIN(x_scale, y_scale);
	x_offset = (1 - ((x_max - x_min) * scale)) * 0.5;
	y_offset = (1 - ((y_max - y_min) * scale)) * 0.5;
	draw_background(data);
	x_tiles = DX;
	idx = -1;
	while (++idx < arr->size - 1)
	{
		if (idx + x_tiles < arr->size)
			draw_line(data, PNT_XY(idx), PNT_XY(idx + x_tiles), PNT(idx)->hsb, PNT(idx + x_tiles)->hsb);
		if (idx % x_tiles != x_tiles - 1)
		{
			if (DDI && idx + x_tiles + 1 < arr->size)
				draw_line(data, PNT_XY(idx), PNT_XY(idx + x_tiles + 1), PNT(idx)->hsb, PNT(idx + x_tiles + 1)->hsb);
			draw_line(data, PNT_XY(idx), PNT_XY(idx + 1), PNT(idx)->hsb, PNT(idx + 1)->hsb);
		}
		if (idx % x_tiles != 0)
			if (DDI && idx + x_tiles - 1 < arr->size)
				draw_line(data, PNT_XY(idx), PNT_XY(idx + x_tiles - 1), ((t_point *)arr->data[idx])->hsb, PNT(idx + x_tiles - 1)->hsb);
	}
	draw_image(data);
}
