#ifndef OBJECT_H
# define OBJECT_H

# include "glad/glad.h"
# include <GLFW/glfw3.h>
# include "shader.h"

struct s_object {
	GLuint			vbo;
	GLuint			vao;
	GLuint			ebo;

	GLfloat 		*vertices;
	GLsizei			nvert;

	GLuint			*indices;
	GLsizei			nind;

	struct s_program *shader;
};

void	object_init(struct s_object *o,
			GLfloat const *verts,
			GLuint const *inds,
			GLsizei nverts,
			GLsizei ninds,
			struct s_program *shader);
void	object_prepare_draw(struct s_object *o);
void	object_draw(struct s_object *o);
void  object_delete(struct s_object *o);
void	object_update(struct s_object *o);

#endif
