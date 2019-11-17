#include "object.h"
#include <stdlib.h>
#include <string.h>
#include <cglm/cglm.h>

#define __unused __attribute__((unused))

void	object_init(struct s_object *o,
			GLfloat const *verts,
			__unused GLuint const *inds,
			GLsizei nverts,
			__unused GLsizei ninds,
			struct s_program *shader)
{
	memset(o, 0, sizeof(*o));

	o->nvert = nverts;
	o->shader = shader;
	o->vertices = malloc(sizeof(GLfloat) * o->nvert);
	memcpy(o->vertices, verts, sizeof(GLfloat) * o->nvert);

	glUseProgram(shader->index);

	glGenVertexArrays(1, &o->vao);
	glGenBuffers(1, &o->vbo);

	glBindVertexArray(o->vao);

	glBindBuffer(GL_ARRAY_BUFFER, o->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * o->nvert, o->vertices,
		GL_STATIC_DRAW);

	size_t stride = sizeof(GLfloat) * 6;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));

	// Unbind buffers to prevent accidental changes
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void	object_prepare_draw(struct s_object *o)
{
	mat4	model;

	glm_mat4_identity(model);

	glm_translate(model, o->pos);

	glm_rotate_x(model, o->rot[0], model);
	glm_rotate_y(model, o->rot[1], model);
	glm_rotate_z(model, o->rot[2], model);

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

void object_delete(struct s_object *o)
{
	glDeleteVertexArrays(1, &o->vao);
	glDeleteBuffers(1, &o->vbo);

	free(o->vertices);
	free(o->indices);
}

void object_set_pos(struct s_object *o, vec3 newpos)
{
	memcpy(o->pos, newpos, sizeof(o->pos));
	o->should_update_mat = 1;
}

void object_rotx(struct s_object *o, float angle)
{
	o->rot[0] = angle;
	o->should_update_mat = 1;
}

void object_roty(struct s_object *o, float angle)
{
	o->rot[1] = angle;
	o->should_update_mat = 1;
}

void object_rotz(struct s_object *o, float angle)
{
	o->rot[2] = angle;
	o->should_update_mat = 1;
}
