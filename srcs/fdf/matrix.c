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

t_matrix *matrix_create(int x, int y, ...) {
	t_matrix *m;
	va_list	va;
	float *cache;

	va_start(va, y);
	m = ft_memalloc(sizeof(*m));
	m->x = x;
	m->y = y;
	m->data = ft_memalloc(sizeof(*(m->data)) * y);
	for (int i = 0; i < y; i++) {
		m->data[i] = ft_memalloc(sizeof(*(*(m->data))) * x);
		cache = va_arg(va, float *);
		for (int j = 0; j < x; j++) {
			m->data[i][j] = cache[j];
		}
	}
	va_end(va);
	return (m);
}

t_matrix *matrix_create_empty(int x, int y) {
	t_matrix *m;

	m = ft_memalloc(sizeof(*m));
	m->x = x;
	m->y = y;
	m->data = ft_memalloc(sizeof(*(m->data)) * y);
	for (int i = 0; i < y; i++) {
		m->data[i] = ft_memalloc(sizeof(*(*(m->data))) * x);
	}
	return (m);
}

t_matrix *matrix_dotproduct(t_matrix *a, t_matrix *b) {
	if (a->x != b->y) {
		return 0;
	}
	t_matrix *result = matrix_create_empty(b->x, a->y);
	for (int i0 = 0; i0 < a->y; i0++) {
		for (int i1 = 0; i1 < b->x; i1++) {
			for (int i2 = 0; i2 < b->y; i2++) {
				result->data[i0][i1] += a->data[i0][i2] * b->data[i2][i1];
			}
		}
	}
	return result;
}

void matrix_free(t_matrix *m) {
	for (int i = 0; i < m->y; i++) {
		free(m->data[i]);
	}
	free(m->data);
	free(m);
}
