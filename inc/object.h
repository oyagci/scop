#ifndef OBJECT_H
# define OBJECT_H

# include "glad/glad.h"
# include <GLFW/glfw3.h>
# include "shader.h"
# include <cglm/cglm.h>

struct s_object {
	vec3			pos;
	vec3			rot;
	GLfloat			scale;
	GLboolean		should_update_mat;

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
			GLfloat const *vdata,
			GLsizei ndata,
			GLsizei stride,
			struct s_program *shader);
void	object_prepare_draw(struct s_object *o);
void	object_draw(struct s_object *o);
void	object_delete(struct s_object *o);
void	object_update(struct s_object *o);

void	object_set_pos(struct s_object *o, vec3 newpos);
void	object_rotx(struct s_object *o, float angle);
void	object_roty(struct s_object *o, float angle);
void	object_rotz(struct s_object *o, float angle);
void	object_set_scale(struct s_object *o, float angle);

#endif
