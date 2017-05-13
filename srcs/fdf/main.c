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

#define DW data->window

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
