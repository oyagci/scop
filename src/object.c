#include "object.h"
#include <stdlib.h>
#include <string.h>

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

	memcpy(o->vertices, verts, sizeof(GLfloat) * o->nvert);
	memcpy(o->indices, inds, sizeof(GLuint) * o->nind);

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

	size_t stride = sizeof(GLfloat) * 8;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
		(void*)(sizeof(GLfloat) * 3));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride,
		(void *)(sizeof(GLfloat) * 6));

	// Unbind buffers to prevent accidental changes
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	o->shader = shader;
}

void	object_prepare_draw(struct s_object *o)
{
	program_use(o->shader);
	glBindVertexArray(o->vao);
}

void	object_draw(struct s_object *o)
{
	object_prepare_draw(o);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void object_delete(struct s_object *o)
{
	glDeleteVertexArrays(1, &o->vao);
	glDeleteBuffers(1, &o->vbo);
	glDeleteBuffers(1, &o->ebo);

	free(o->vertices);
	free(o->indices);
}
