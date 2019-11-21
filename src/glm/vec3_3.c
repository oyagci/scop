/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 15:37:59 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 15:38:08 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "glm.h"

void	glm_t_vec3_cross(t_vec3 a, t_vec3 b, t_vec3 dest)
{
	dest[0] = a[1] * b[2] - a[2] * b[1];
	dest[1] = a[2] * b[0] - a[0] * b[2];
	dest[2] = a[0] * b[1] - a[1] * b[0];
}

void	glm_cross(t_vec3 a, t_vec3 b, t_vec3 d)
{
	glm_t_vec3_cross(a, b, d);
}

void	glm_t_vec3_crossn(t_vec3 a, t_vec3 b, t_vec3 dest)
{
	glm_t_vec3_cross(a, b, dest);
	glm_t_vec3_normalize(dest);
}

void	glm_t_vec3_copy(t_vec3 a, t_vec3 dest)
{
	dest[0] = a[0];
	dest[1] = a[1];
	dest[2] = a[2];
}
