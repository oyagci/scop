/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:32 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 14:55:38 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "scop.h"

void	mouse_init(t_mouse *mouse)
{
	mouse->first_mouse = 1;
	mouse->last_x = WIDTH / 2;
	mouse->last_y = HEIGHT / 2;
	mouse->pitch = 0.0f;
	mouse->yaw = -90.0f;
	mouse->sensitivity = 2.6f;
}

void	render_win_init(t_render_win *window)
{
	window->width = WIDTH;
	window->height = HEIGHT;
}

void	gl_camera_init(t_gl_camera *camera)
{
	memcpy(&camera->front, (vec3){ 0.0f, 0.0f, -1.0f }, sizeof(vec3));
	memcpy(&camera->up, (vec3){ 0.0f, 1.0f, 0.0f }, sizeof(vec3));
	memcpy(&camera->pos, (vec3){ 0.0f, 0.0f, 6.0f }, sizeof(vec3));
}

void	obj_info_init(t_obj_info *obj)
{
	memset(obj->pos, 0, sizeof(vec3));
	memset(obj->rot, 0, sizeof(vec3));
	obj->opacity_value = 0;
	obj->opacity = GL_FALSE;
}

void	engine_init(t_engine *engine)
{
	engine->delta_time = 0;
	mouse_init(&engine->mouse);
	render_win_init(&engine->window);
	gl_camera_init(&engine->camera);
	obj_info_init(&engine->obj);
}
