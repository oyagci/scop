/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:32 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 14:55:38 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "glm.h"
#include <math.h>

void	glm_vec3_add(vec3 a, vec3 b, vec3 dest)
{
	dest[0] = a[0] + b[0];
	dest[1] = a[1] + b[1];
	dest[2] = a[2] + b[2];
}

void	glm_vec3_sub(vec3 a, vec3 b, vec3 dest)
{
	dest[0] = a[0] - b[0];
	dest[1] = a[1] - b[1];
	dest[2] = a[2] - b[2];
}

float	glm_vec3_dot(vec3 a, vec3 b)
{
	return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
}

float	glm_vec3_norm2(vec3 v)
{
	return (glm_vec3_dot(v, v));
}

float	glm_vec3_norm(vec3 v)
{
	return (sqrtf(glm_vec3_norm2(v)));
}

void	glm_vec3_scale(vec3 v, float s, vec3 dest)
{
	dest[0] = v[0] * s;
	dest[1] = v[1] * s;
	dest[2] = v[2] * s;
}

void	glm_vec3_normalize(vec3 v)
{
	float	norm;

	norm = glm_vec3_norm(v);
	if (norm == 0.0f)
	{
		v[0] = 0.0f;
		v[1] = 0.0f;
		v[2] = 0.0f;
		return ;
	}
	glm_vec3_scale(v, 1.0f / norm, v);
}

void	glm_normalize(vec3 v)
{
	return (glm_vec3_normalize(v));
}

void	glm_vec3_cross(vec3 a, vec3 b, vec3 dest)
{
	dest[0] = a[1] * b[2] - a[2] * b[1];
	dest[1] = a[2] * b[0] - a[0] * b[2];
	dest[2] = a[0] * b[1] - a[1] * b[0];
}

void	glm_cross(vec3 a, vec3 b, vec3 d)
{
	glm_vec3_cross(a, b, d);
}

void	glm_vec3_crossn(vec3 a, vec3 b, vec3 dest)
{
	glm_vec3_cross(a, b, dest);
	glm_vec3_normalize(dest);
}

void glm_vec3_copy(vec3 a, vec3 dest)
{
	dest[0] = a[0];
	dest[1] = a[1];
	dest[2] = a[2];
}
