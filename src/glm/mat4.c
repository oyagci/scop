/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:32 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 15:10:08 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "glm.h"
#include <math.h>

void	glm_mat4_copy(mat4 mat, mat4 dest)
{
	dest[0][0] = mat[0][0];
	dest[1][0] = mat[1][0];
	dest[0][1] = mat[0][1];
	dest[1][1] = mat[1][1];
	dest[0][2] = mat[0][2];
	dest[1][2] = mat[1][2];
	dest[0][3] = mat[0][3];
	dest[1][3] = mat[1][3];
	dest[2][0] = mat[2][0];
	dest[3][0] = mat[3][0];
	dest[2][1] = mat[2][1];
	dest[3][1] = mat[3][1];
	dest[2][2] = mat[2][2];
	dest[3][2] = mat[3][2];
	dest[2][3] = mat[2][3];
	dest[3][3] = mat[3][3];
}

void	glm_mat4_identity(mat4 mat)
{
	glm_mat4_copy((mat4){{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}, mat);
}

void	glm_mat4_zero(mat4 mat)
{
	glm_mat4_copy((mat4){{0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}}, mat);
}

void	glm_translate(mat4 m, vec3 v)
{
	vec4 v1;
	vec4 v2;
	vec4 v3;

	glm_vec4_scale(m[0], v[0], v1);
	glm_vec4_scale(m[1], v[1], v2);
	glm_vec4_scale(m[2], v[2], v3);
	glm_vec4_add(v1, m[3], m[3]);
	glm_vec4_add(v2, m[3], m[3]);
	glm_vec4_add(v3, m[3], m[3]);
}

void	glm_perspective(float fovy,
	float aspect,
	float const near_far[2],
	mat4 dest)
{
	float		f;
	float		fn;
	float const	near_val = near_far[0];
	float const	far_val = near_far[1];

	glm_mat4_zero(dest);
	f = 1.0f / tanf(fovy * 0.5f);
	fn = 1.0f / (near_val - far_val);
	dest[0][0] = f / aspect;
	dest[1][1] = f;
	dest[2][2] = (near_val + far_val) * fn;
	dest[2][3] = -1.0f;
	dest[3][2] = 2.0f * near_val * far_val * fn;
}
