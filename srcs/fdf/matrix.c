/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achi <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/30 15:49:40 by achi              #+#    #+#             */
/*   Updated: 2017/01/30 15:49:42 by achi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <fdf.h>

#define I_DO_DECLARE int i;int j;t_matrix *m;va_list va;float *cache;

t_matrix	*matrix_create(int x, int y, ...)
{
	I_DO_DECLARE;
	va_start(va, y);
	m = ft_memalloc(sizeof(*m));
	m->x = x;
	m->y = y;
	m->data = ft_memalloc(sizeof(*(m->data)) * y);
	i = 0;
	while (i < y)
	{
		m->data[i] = ft_memalloc(sizeof(*(*(m->data))) * x);
		cache = va_arg(va, float *);
		j = 0;
		while (j < x)
		{
			m->data[i][j] = cache[j];
			j++;
		}
		i++;
	}
	va_end(va);
	return (m);
}

t_matrix	*matrix_create_empty(int x, int y)
{
	t_matrix	*m;
	int			i;

	m = ft_memalloc(sizeof(*m));
	m->x = x;
	m->y = y;
	m->data = ft_memalloc(sizeof(*(m->data)) * y);
	i = 0;
	while (i < y)
	{
		m->data[i] = ft_memalloc(sizeof(*(*(m->data))) * x);
		i++;
	}
	return (m);
}

t_matrix	*matrix_dotproduct(t_matrix *a, t_matrix *b)
{
	int			i0;
	int			i1;
	int			i2;
	t_matrix	*result;

	if (a->x != b->y)
		return (0);
	result = matrix_create_empty(b->x, a->y);
	i0 = 0;
	while (i0 < a->y)
	{
		i1 = 0;
		while (i1 < b->x)
		{
			i2 = 0;
			while (i2 < b->y)
			{
				result->data[i0][i1] += a->data[i0][i2] * b->data[i2][i1];
				i2++;
			}
			i1++;
		}
		i0++;
	}
	return (result);
}

void		matrix_free(t_matrix *m)
{
	int	i;

	i = 0;
	while (i < m->y)
	{
		free(m->data[i]);
		i++;
	}
	free(m->data);
	free(m);
}
