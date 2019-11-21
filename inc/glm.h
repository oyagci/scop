/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:32 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 15:14:48 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLM_H
# define GLM_H

typedef float	t_vec3[3];
typedef float	t_vec4[4];
typedef t_vec4	t_mat4[4];

void			glm_t_vec3_add(t_vec3 a, t_vec3 b, t_vec3 dest);
void			glm_t_vec3_sub(t_vec3 a, t_vec3 b, t_vec3 dest);
void			glm_t_vec3_copy(t_vec3 a, t_vec3 dest);
float			glm_t_vec3_dot(t_vec3 a, t_vec3 b);
float			glm_t_vec3_norm2(t_vec3 v);
float			glm_t_vec3_norm(t_vec3 v);
void			glm_t_vec3_scale(t_vec3 v, float s, t_vec3 dest);
void			glm_t_vec3_normalize(t_vec3 v);
void			glm_t_vec3_cross(t_vec3 a, t_vec3 b, t_vec3 dest);
void			glm_t_vec3_crossn(t_vec3 a, t_vec3 b, t_vec3 dest);
void			glm_cross(t_vec3 a, t_vec3 b, t_vec3 d);
void			glm_normalize(t_vec3 v);

void			glm_t_vec4_scale(t_vec4 v, float s, t_vec4 dest);
void			glm_t_vec4_add(t_vec4 a, t_vec4 b, t_vec4 dest);
void			glm_t_vec4_copy(t_vec4 v, t_vec4 dest);

void			glm_t_mat4_zero(t_mat4 m);
void			glm_t_mat4_copy(t_mat4 mat, t_mat4 dest);
void			glm_t_mat4_identity(t_mat4 mat);
void			glm_translate(t_mat4 m, t_vec3 v);
void			glm_scale_uni(t_mat4 m, float s);

void			glm_rotate_x(t_mat4 m, float angle, t_mat4 dest);
void			glm_rotate_y(t_mat4 m, float angle, t_mat4 dest);
void			glm_rotate_z(t_mat4 m, float angle, t_mat4 dest);

void			glm_lookat(t_vec3 eye,
					t_vec3 center,
					t_vec3 up,
					t_mat4 dest);
void			glm_perspective(float fovy,
					float aspect,
					float const near_far[2],
					t_mat4 dest);
void			glm_mul_rot(t_mat4 m1, t_mat4 m2, t_mat4 dest);

float			glm_rad(float deg);

#endif
