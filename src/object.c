/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:32 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 15:40:04 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "object.h"
#include <stdlib.h>
#include <string.h>
#include "common.h"

void	object_init(struct s_object *o,
			GLfloat const *vdata,
			GLsizei ndata,
			GLsizei stride)
{
	memset(o, 0, sizeof(*o));
	o->scale = 1.0f;
	o->nvert = ndata;
	o->shader = 0;
	o->vertices = malloc_abort(sizeof(GLfloat) * o->nvert);
	memcpy(o->vertices, vdata, sizeof(GLfloat) * o->nvert);
	glGenVertexArrays(1, &o->vao);
	glBindVertexArray(o->vao);
	glGenBuffers(1, &o->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, o->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * o->nvert, o->vertices,
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
		(void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,
		(void*)(sizeof(float) * 6));
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void	object_prepare_draw(struct s_object *o)
{
	t_mat4	model;

	glm_t_mat4_identity(model);
	glm_translate(model, o->pos);
	glm_rotate_x(model, o->rot[0], model);
	glm_rotate_y(model, o->rot[1], model);
	glm_rotate_z(model, o->rot[2], model);
	glm_scale_uni(model, o->scale);
	program_use(o->shader);
	glUniformMatrix4fv(glGetUniformLocation(o->shader->index, "model"), 1,
		GL_FALSE, model[0]);
	glBindVertexArray(o->vao);
}

void	object_draw(struct s_object *o)
{
	object_prepare_draw(o);
	glDrawArrays(GL_TRIANGLES, 0, o->nvert);
}

void	object_delete(struct s_object *o)
{
	glDeleteVertexArrays(1, &o->vao);
	glDeleteBuffers(1, &o->vbo);
	free(o->vertices);
	free(o->indices);
}
