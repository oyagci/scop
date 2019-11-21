#include "shader.h"

void	shader_set_vec3(struct s_program *p, char const *name, vec3 v)
{
	program_use(p);
	glUniform3fv(glGetUniformLocation(p->index, name), 1, v);
}

void	shader_set_mat4(struct s_program *p, char const *name, mat4 m)
{
	program_use(p);
	glUniformMatrix4fv(glGetUniformLocation(p->index, name), 1,
		GL_FALSE, *m);
}

void	shader_set_float(struct s_program *p, char const *name, float f)
{
	program_use(p);
	glUniform1f(glGetUniformLocation(p->index, name), f);
}
