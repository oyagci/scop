/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_set.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:32 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 14:55:39 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "object.h"
#include <string.h>

void	object_set_pos(struct s_object *o, t_vec3 newpos)
{
	memcpy(o->pos, newpos, sizeof(o->pos));
}

void	object_rotx(struct s_object *o, float angle)
{
	o->rot[0] += glm_rad(angle);
}

void	object_roty(struct s_object *o, float angle)
{
	o->rot[1] += glm_rad(angle);
}

void	object_rotz(struct s_object *o, float angle)
{
	o->rot[2] += glm_rad(angle);
}

void	object_set_scale(struct s_object *o, float angle)
{
	o->scale = angle;
}
