#include <string.h>
#include "shader.h"
#include <stdio.h>

void	shader_init(struct s_shader *s, GLenum shader_type)
{
	memset(s, 0, sizeof(*s));
	s->type = shader_type;
	s->index = glCreateShader(shader_type);
}

void	shader_load_source(struct s_shader *s, const char *const src)
{
	glShaderSource(s->index, 1, &src, NULL);
}

void	shader_compile(struct s_shader *s)
{
	glCompileShader(s->index);
}

int		shader_compile_success(struct s_shader *s)
{
	GLint success = 0;

	glGetShaderiv(s->index, GL_COMPILE_STATUS, &success);
	return (success);
}

void	shader_infolog(struct s_shader *s, char *buf, size_t bufsize)
{
	glGetShaderInfoLog(s->index, bufsize, NULL, buf);
}

void	shader_delete(struct s_shader *s)
{
	glDeleteShader(s->index);
}

