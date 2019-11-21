/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex_container.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:32 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 14:55:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj.h"
#include "common.h"

int	vertex_container_init(t_vertex_container *vc)
{
	vc->data = calloc_abort(500, sizeof(t_vertex));
	vc->capacity = 500;
	vc->size = 0;
	return (0);
}

int	vertex_container_add(t_vertex_container *vc, t_vertex *v)
{
	t_vertex	*d;

	if (vc->size >= vc->capacity)
	{
		d = realloc_abort(vc->data,
			sizeof(t_vertex) * (vc->capacity + 500));
		vc->data = d;
		vc->capacity += 500;
	}
	memcpy(vc->data + vc->size, v, sizeof(t_vertex));
	vc->size += 1;
	return (0);
}
