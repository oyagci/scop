/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:32 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 15:03:58 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLM_H
# define GLM_H

typedef float	vec3[3];
typedef float	vec4[4];
typedef vec4	mat4[4];

void			glm_vec3_add(vec3 a, vec3 b, vec3 dest);
void			glm_vec3_sub(vec3 a, vec3 b, vec3 dest);
void			glm_vec3_copy(vec3 a, vec3 dest);
float			glm_vec3_dot(vec3 a, vec3 b);
float			glm_vec3_norm2(vec3 v);
float			glm_vec3_norm(vec3 v);
void			glm_vec3_scale(vec3 v, float s, vec3 dest);
void			glm_vec3_normalize(vec3 v);
void			glm_vec3_cross(vec3 a, vec3 b, vec3 dest);
void			glm_vec3_crossn(vec3 a, vec3 b, vec3 dest);
void			glm_cross(vec3 a, vec3 b, vec3 d);
void			glm_normalize(vec3 v);

void			glm_vec4_scale(vec4 v, float s, vec4 dest);
void			glm_vec4_add(vec4 a, vec4 b, vec4 dest);
void			glm_vec4_copy(vec4 v, vec4 dest);

void			glm_mat4_zero(mat4 m);
void			glm_mat4_copy(mat4 mat, mat4 dest);
void			glm_mat4_identity(mat4 mat);
void			glm_translate(mat4 m, vec3 v);
void			glm_scale_uni(mat4 m, float s);

void			glm_rotate_x(mat4 m, float angle, mat4 dest);
void			glm_rotate_y(mat4 m, float angle, mat4 dest);
void			glm_rotate_z(mat4 m, float angle, mat4 dest);

void			glm_lookat(vec3 eye,
					vec3 center,
					vec3 up,
					mat4 dest);
void			glm_perspective(float fovy,
					float aspect,
					float const near_far[2],
					mat4 dest);
void			glm_mul_rot(mat4 m1, mat4 m2, mat4 dest);

float			glm_rad(float deg);

#endif
