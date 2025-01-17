/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_delete.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:32 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 14:55:39 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "obj.h"

void	triangle_container_delete(t_triangle_container *tc)
{
	free(tc->data);
}

void	vertex_container_delete(t_vertex_container *vc)
{
	free(vc->data);
}

void	face_container_delete(t_face_container *fc)
{
	size_t	i;

	i = 0;
	while (i < fc->size)
	{
		free(fc->data[i].indices);
		i += 1;
	}
	free(fc->data);
}

void	obj_delete(t_obj *obj)
{
	free(obj->data);
	face_container_delete(&obj->faces);
	triangle_container_delete(&obj->triangles);
	vertex_container_delete(&obj->vertices);
}
