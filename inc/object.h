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

	GLfloat 		*vertices;
	GLsizei			nvert;

	struct s_program *shader;
};

void	object_init(struct s_object *o,
			GLfloat const *verts,
			GLsizei nverts,
			struct s_program *shader);
void	object_prepare_draw(struct s_object *o);
void	object_draw(struct s_object *o);
void	object_delete(struct s_object *o);
void	object_update(struct s_object *o);

void	object_set_pos(struct s_object *o, vec3 newpos);
void	object_rotx(struct s_object *o, float angle);
void	object_roty(struct s_object *o, float angle);
void	object_rotz(struct s_object *o, float angle);
void	object_scale(struct s_object *o, float scale);

#endif
