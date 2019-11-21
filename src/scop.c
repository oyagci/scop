/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:32 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 15:04:50 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "scop.h"
#include "shader.h"
#include "object.h"
#include "obj.h"

void		scop_shader_init(struct s_program *shader)
{
	t_mat4	projection;

	glm_perspective(45.0f,
		(float)g_engine.window.width / (float)g_engine.window.height,
		(float[]){ 0.1f, 100.0f }, projection);
	shader_set_t_vec3(shader, "lightColor", (t_vec3){ 1.0f, 1.0f, 1.0f });
	shader_set_t_vec3(shader, "lightPos", (t_vec3){ 5.0f, 1.0f, 5.0f });
	shader_set_float(shader, "opacity", 0.0f);
	shader_set_t_mat4(shader, "proj", projection);
}

void		scop_run(GLFWwindow *window, struct s_object *object)
{
	while (!glfwWindowShouldClose(window))
	{
		update_delta_time();
		print_fps();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		process_input(window);
		update_camera(1, object->shader);
		object_set_pos(object, g_engine.obj.pos);
		object_roty(object, 10.f * g_engine.delta_time);
		object_rotx(object, g_engine.obj.rot[0]);
		object_roty(object, g_engine.obj.rot[1]);
		object_rotz(object, g_engine.obj.rot[2]);
		g_engine.obj.rot[0] = 0.0f;
		g_engine.obj.rot[1] = 0.0f;
		g_engine.obj.rot[2] = 0.0f;
		update_opacity(object);
		object_draw(object);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

int			scop(GLFWwindow *window, char const *filename)
{
	struct s_program	shader;
	struct s_object		object;
	t_gltri				*vdata;
	GLuint				stonewall;
	t_obj				obj;

	if (scop_obj_init(filename, &obj) < 0)
		return (1);
	vdata = obj_get_triangles_arr(&obj);
	obj_delete(&obj);
	if (shader_from_files("./shaders/vertex.glsl", "./shaders/fragment.glsl",
		&shader) < 0)
		return (1);
	scop_shader_init(&shader);
	object_init(&object, (float *)vdata, obj.triangles.size * 3 * 9,
	   sizeof(GLfloat) * 9,	&shader);
	object_set_pos(&object, g_engine.obj.pos);
	free(vdata);
	stonewall = texture_create(GL_TEXTURE0, "img/chaton_roux.bmp");
	scop_run(window, &object);
	object_delete(&object);
	program_delete(&shader);
	glDeleteTextures(1, &stonewall);
	return (0);
}
