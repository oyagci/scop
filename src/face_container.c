/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   face_container.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:32 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 14:55:38 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj.h"
#include "common.h"

int	face_container_init(t_face_container *vc)
{
	vc->data = calloc_abort(500, sizeof(t_face));
	vc->capacity = 500;
	vc->size = 0;
	return (0);
}

int	face_container_add(t_face_container *vc, t_face *v)
{
	t_face	*d;

	if (vc->size >= vc->capacity)
	{
		d = realloc_abort(vc->data,
			sizeof(t_face) * (vc->capacity + 500));
		vc->data = d;
		vc->capacity += 500;
	}
	memcpy(vc->data + vc->size, v, sizeof(t_face));
	vc->size += 1;
	return (0);
}
