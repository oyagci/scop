/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat4_rot.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 15:08:36 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 15:10:05 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "glm.h"
#include <math.h>

void	glm_rotate_x(mat4 m, float angle, mat4 dest)
{
	mat4	t;
	float	c;
	float	s;

	glm_mat4_identity(t);
	c = cosf(angle);
	s = sinf(angle);
	t[1][1] = c;
	t[1][2] = s;
	t[2][1] = -s;
	t[2][2] = c;
	glm_mul_rot(m, t, dest);
}

void	glm_rotate_y(mat4 m, float angle, mat4 dest)
{
	mat4	t;
	float	c;
	float	s;

	glm_mat4_identity(t);
	c = cosf(angle);
	s = sinf(angle);
	t[0][0] = c;
	t[0][2] = -s;
	t[2][0] = s;
	t[2][2] = c;
	glm_mul_rot(m, t, dest);
}

void	glm_rotate_z(mat4 m, float angle, mat4 dest)
{
	mat4	t;
	float	c;
	float	s;

	glm_mat4_identity(t);
	c = cosf(angle);
	s = sinf(angle);
	t[0][0] = c;
	t[0][1] = s;
	t[1][0] = -s;
	t[1][1] = c;
	glm_mul_rot(m, t, dest);
}

void	glm_mul_rot(mat4 m1, mat4 m2, mat4 dest)
{
	float const a[4][4] = {
		{ m1[0][0], m1[0][1], m1[0][2], m1[0][3] }, { m1[1][0], m1[1][1],
		m1[1][2], m1[1][3] }, { m1[2][0], m1[2][1], m1[2][2],
		m1[2][3] }, { m1[3][0], m1[3][1], m1[3][2], m1[3][3] } };
	float const b[3][3] = {
		{ m2[0][0], m2[0][1], m2[0][2] }, { m2[1][0], m2[1][1], m2[1][2] },
		{ m2[2][0], m2[2][1], m2[2][2] } };

	dest[0][0] = a[0][0] * b[0][0] + a[1][0] * b[0][1] + a[2][0] * b[0][2];
	dest[0][1] = a[0][1] * b[0][0] + a[1][1] * b[0][1] + a[2][1] * b[0][2];
	dest[0][2] = a[0][2] * b[0][0] + a[1][2] * b[0][1] + a[2][2] * b[0][2];
	dest[0][3] = a[0][3] * b[0][0] + a[1][3] * b[0][1] + a[2][3] * b[0][2];
	dest[1][0] = a[0][0] * b[1][0] + a[1][0] * b[1][1] + a[2][0] * b[1][2];
	dest[1][1] = a[0][1] * b[1][0] + a[1][1] * b[1][1] + a[2][1] * b[1][2];
	dest[1][2] = a[0][2] * b[1][0] + a[1][2] * b[1][1] + a[2][2] * b[1][2];
	dest[1][3] = a[0][3] * b[1][0] + a[1][3] * b[1][1] + a[2][3] * b[1][2];
	dest[2][0] = a[0][0] * b[2][0] + a[1][0] * b[2][1] + a[2][0] * b[2][2];
	dest[2][1] = a[0][1] * b[2][0] + a[1][1] * b[2][1] + a[2][1] * b[2][2];
	dest[2][2] = a[0][2] * b[2][0] + a[1][2] * b[2][1] + a[2][2] * b[2][2];
	dest[2][3] = a[0][3] * b[2][0] + a[1][3] * b[2][1] + a[2][3] * b[2][2];
	dest[3][0] = a[3][0];
	dest[3][1] = a[3][1];
	dest[3][2] = a[3][2];
	dest[3][3] = a[3][3];
}
