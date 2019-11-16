#include "object.h"
#include <stdlib.h>
#include <string.h>
#include <cglm/cglm.h>

void	object_init(struct s_object *o,
			GLfloat const *verts,
			GLuint const *inds,
			GLsizei nverts,
			GLsizei ninds,
			struct s_program *shader)
{
	memset(o, 0, sizeof(*o));

	o->nvert = nverts;
	o->nind = ninds;

	o->vertices = malloc(sizeof(GLfloat) * o->nvert);
	o->indices = malloc(sizeof(GLuint) * o->nind);

	o->shader = shader;

	memcpy(o->vertices, verts, sizeof(GLfloat) * o->nvert);
	memcpy(o->indices, inds, sizeof(GLuint) * o->nind);

	glUseProgram(shader->index);

	glGenVertexArrays(1, &o->vao);
	glGenBuffers(1, &o->vbo);
	glGenBuffers(1, &o->ebo);

	glBindVertexArray(o->vao);

	glBindBuffer(GL_ARRAY_BUFFER, o->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * o->nvert, o->vertices,
		GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * o->nind, o->indices,
		GL_STATIC_DRAW);

	size_t stride = sizeof(GLfloat) * 4;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, stride, (void*)0);

	// Unbind buffers to prevent accidental changes
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void	object_prepare_draw(struct s_object *o)
{
	if (o->should_update_mat) {
		mat4	model;

		glm_mat4_identity(model);

		glm_translate(model, o->pos);

		glm_rotate_x(model, o->rot[0], model);
		glm_rotate_y(model, o->rot[1], model);
		glm_rotate_z(model, o->rot[2], model);

		glUniformMatrix4fv(glGetUniformLocation(o->shader->index, "model"), 1,
			GL_FALSE, model[0]);

		o->should_update_mat = 0;
	}

	program_use(o->shader);
	glBindVertexArray(o->vao);
}

void	object_draw(struct s_object *o)
{
	object_prepare_draw(o);
	glDrawElements(GL_TRIANGLES, o->nind, GL_UNSIGNED_INT, 0);
}

void object_delete(struct s_object *o)
{
	glDeleteVertexArrays(1, &o->vao);
	glDeleteBuffers(1, &o->vbo);
//	glDeleteBuffers(1, &o->ebo);

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
