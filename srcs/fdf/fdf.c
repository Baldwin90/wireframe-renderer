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

int		key_hook(int key_code, t_mapdata *data) {
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
		} else if (data->increment > 1) {
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
		} else if (data->increment >= 1) {
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

void	create_mlx(t_mapdata *data) {
	data->window->mlx = mlx_init();
	data->window->win = mlx_new_window(data->window->mlx, SCREENSIZE, SCREENSIZE, "FDF");
	data->window->img = mlx_new_image(data->window->mlx, SCREENSIZE, SCREENSIZE);
	data->window->pixel = mlx_get_data_addr(data->window->img, &(data->window->bpp), &(data->window->line_size), &(data->window->endian));
	data->window->bpp /= 8;
	mlx_expose_hook(data->window->win, &draw_image, data);
	mlx_key_hook(data->window->win, &key_hook, data);
}

int		create_point(t_mapdata *data, char **fields, int field_count, int z) {
	char	**point;
	int		point_data_count;
	int		rgb_val;

	for (int x = 0; x < field_count; x += 1) {
		point = ft_strsplit(fields[x], ',');
		ARR_SIZE(point, point_data_count);
		// point_data_count = ft_arrsize(point, sizeof(*point));
		switch (point_data_count) {
			case 1:
				data_addpoint(data, (float[]){x, ft_atoi(point[0]), z}, (float[]){0, 0, 1});
				break;
			case 2:
				if ((rgb_val = ft_htoi(point[1])) == -1 || rgb_val > RGB_MAX) {
					// ERROR, RGB value is not valid
					return (FALSE);
				}
				// data_addpoint(data, (float[]){x, ft_atoi(point[0]), z}, RBGToHSB(rgb_val)); // I DON'T KNOW HOW TO USE THE LAST FUNCTION
				data_addpoint(data, (float[]){x, ft_atoi(point[0]), z}, (float[]){0, 0, 1}); // PLACEHOLDER LINE
				break;
			default:
				// ERROR, too many arguments in the point's data
				return (FALSE);
				break;
		}
	}
	return (TRUE);
}

// if it's white data_addpoint(data, (float[]){x, y, z}, (float[]){0, 0, 1});
// if it has color data_addpoint(data, (float[]){x, y, z}, RBGToHSB(color));

//YOUR JOB :3
void	map_fill(t_mapdata *data, int fd) {
	int		gnl_res;
	char	*line;
	char	**fields;
	int		field_count;

	line = NULL;
	// YOU HAVE TO FREE THE FIELDS AND THE POINT DATA!!!!!!!
	gnl_res = get_next_line(fd, &line);
	for (int z = 0; gnl_res == 1; z += 1, gnl_res = get_next_line(fd, &line)) {
		fields = ft_strsplit(line, ' ');
		ARR_SIZE(fields, field_count);
		// field_count = ft_arrsize(fields, sizeof(*fields)); // replaces for loop... maybe
		// for (field_count = 0; fields[field_count] != NULL; field_count += 1) {
		// 	;
		// }
		if (data->x_size < 0) {
			data->x_size = field_count;
		} else if (field_count != data->x_size) {
			// ERROR, not all rows are the same length or they are zero
			fdf_error(3, line, fd, data);
		} else if (field_count == 0) {
			// ERROR, a row has no points
			fdf_error(4, line, fd, data);
		}
		if (!create_point(data, fields, field_count, z)) {
			// ERROR, issue creating point
			fdf_error(5, line, fd, data);
		}
	}
	if (line != NULL)
		free(line);
	// data->x_size = 19;
	// int arr[209] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,0,0,10,10,0,0,0,10,10,10,10,10,0,0,0,0,0,10,10,0,0,10,10,0,0,0,0,0,0,0,10,10,0,0,0,0,10,10,0,0,10,10,0,0,0,0,0,0,0,10,10,0,0,0,0,10,10,10,10,10,10,0,0,0,0,10,10,10,10,0,0,0,0,0,0,10,10,10,10,10,0,0,0,10,10,0,0,0,0,0,0,0,0,0,0,0,0,10,10,0,0,0,10,10,0,0,0,0,0,0,0,0,0,0,0,0,10,10,0,0,0,10,10,10,10,10,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	// for (int i = 0; i < 209; i++) {
	// 	data_addpoint(data, (float []){(int)(i % data->x_size), arr[i], (int)(i / data->x_size)}, (float []){(float)i / 209, 1, 1});
	// }
}

void	fdf_error(int fdf_err_num, char *line, int fd, t_mapdata *data) {
	if (line) {
		free(line);
	}
	if (fd != -1) {
		if (close(fd) == -1) {
			fdf_error(fdf_err_num + 1000, NULL, -1, data);
		}
	}
	if (data != NULL) {
		data_free(data);
	}
	exit(fdf_err_num);
}

int		main(int argc, char const *argv[]) {
	// (void)argc;
	// (void)argv;
	// t_matrix *x = matrix_create(3, 3, (float []){1, 2, 3}, (float []){4,5,6}, (float []){7,8,9});
	// t_matrix *y = matrix_create(3, 3, (float []){10, 11, 12}, (float []){13,14,15}, (float []){16,17,18});
	// t_matrix *z = matrix_dotproduct(x, y);
	// matrix_free(x);
	// matrix_free(y);
	// matrix_free(z);

	int fd;

	if (argc != 2) {
		// ERROR, incorrect number of arguments
		fdf_error(1, NULL, -1, NULL);
	}
	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		// ERROR, unable to open file
		fdf_error(2, NULL, -1, NULL);
	}
	t_mapdata *data = data_create(0x141116);
	map_fill(data, fd);
	if (close(fd) == -1) {
		fdf_error(1000, NULL, -1, data);
	}
	create_mlx(data);
	draw_fdf(data);
	mlx_loop(data->window->mlx);
	return (0);
}
