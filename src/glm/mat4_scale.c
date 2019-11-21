/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat4_scale.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 15:09:07 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 15:09:27 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "glm.h"

void	glm_scale_to(t_mat4 m, t_vec3 v, t_mat4 dest)
{
	glm_t_vec4_scale(m[0], v[0], dest[0]);
	glm_t_vec4_scale(m[1], v[1], dest[1]);
	glm_t_vec4_scale(m[2], v[2], dest[2]);
	glm_t_vec4_copy(m[3], dest[3]);
}

void	glm_scale_uni(t_mat4 m, float s)
{
	glm_scale_to(m, (t_vec3){ s, s, s }, m);
}
