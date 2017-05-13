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

char	*ft_ftoa(float f)
{
	char	buf[48];

	snprintf(buf, sizeof(buf), "%.2f", f);
	return (ft_strdup(buf));
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
