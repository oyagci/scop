/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:32 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 14:55:40 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"
#include "shader.h"
#include "object.h"

void	update_camera(size_t n, struct s_program *shaders)
{
	t_mat4	view;
	t_vec3	cam_target;
	size_t	i;

	glm_t_vec3_add(g_engine.camera.front, g_engine.camera.pos, cam_target);
	glm_lookat(g_engine.camera.pos, cam_target, g_engine.camera.up, view);
	i = 0;
	while (i < n)
	{
		shader_set_t_mat4(shaders + i, "view", view);
		shader_set_t_vec3(shaders + i, "viewPos", g_engine.camera.pos);
		i += 1;
	}
}

void	update_opacity(struct s_object *o)
{
	float	speed;

	speed = 1.0f * g_engine.delta_time;
	if (g_engine.obj.opacity_dir > 0)
	{
		g_engine.obj.opacity_value += speed;
		shader_set_float(o->shader, "opacity", g_engine.obj.opacity_value);
		if (g_engine.obj.opacity_value >= 1.0f)
		{
			g_engine.obj.opacity_dir = 0;
			g_engine.obj.opacity_value = 1.0f;
		}
	}
	else if (g_engine.obj.opacity_dir < 0)
	{
		g_engine.obj.opacity_value -= speed;
		shader_set_float(o->shader, "opacity", g_engine.obj.opacity_value);
		if (g_engine.obj.opacity_value <= 0.05f)
		{
			g_engine.obj.opacity_dir = 0;
			g_engine.obj.opacity_value = 0.0f;
		}
	}
}

void	update_delta_time(void)
{
	static float	delta_time = 0.0f;
	static float	last_frame = 0.0f;
	float			current_frame;

	current_frame = glfwGetTime();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;
	g_engine.delta_time = delta_time;
}
