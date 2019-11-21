/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:32 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 15:38:10 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "glm.h"
#include <math.h>

float	glm_t_vec3_norm(t_vec3 v)
{
	return (sqrtf(glm_t_vec3_norm2(v)));
}

void	glm_t_vec3_scale(t_vec3 v, float s, t_vec3 dest)
{
	dest[0] = v[0] * s;
	dest[1] = v[1] * s;
	dest[2] = v[2] * s;
}

void	glm_t_vec3_normalize(t_vec3 v)
{
	float	norm;

	norm = glm_t_vec3_norm(v);
	if (norm == 0.0f)
	{
		v[0] = 0.0f;
		v[1] = 0.0f;
		v[2] = 0.0f;
		return ;
	}
	glm_t_vec3_scale(v, 1.0f / norm, v);
}

void	glm_normalize(t_vec3 v)
{
	return (glm_t_vec3_normalize(v));
}
