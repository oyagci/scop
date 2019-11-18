#include <string.h>
#include "shader.h"

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

void	program_init(struct s_program *p)
{
	memset(p, 0, sizeof(*p));
	p->index = glCreateProgram();
}

#include <stdio.h>
void	program_create(struct s_program *p,
		const char *const vertsrc,
		const char *const fragsrc)
{
	program_init(p);
	shader_init(&p->vertex, GL_VERTEX_SHADER);
	shader_init(&p->fragment, GL_FRAGMENT_SHADER);

	shader_load_source(&p->vertex, vertsrc);
	shader_compile(&p->vertex);

	if (!shader_compile_success(&p->vertex))
	{
		char log[512];

		shader_infolog(&p->vertex, log, sizeof(log));
		fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", log);
	}

	shader_load_source(&p->fragment, fragsrc);
	shader_compile(&p->fragment);

	if (!shader_compile_success(&p->fragment))
	{
		char log[512];

		shader_infolog(&p->fragment, log, sizeof(log));
		fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", log);
	}

	glAttachShader(p->index, p->vertex.index);
	glAttachShader(p->index, p->fragment.index);

	glLinkProgram(p->index);

	shader_delete(&p->vertex);
	shader_delete(&p->fragment);
}

void	program_delete(struct s_program *p)
{
	glDeleteProgram(p->index);
}

void program_use(struct s_program *p)
{
	glUseProgram(p->index);
}

void shader_set_vec3(struct s_program *p, char const *name, vec3 v)
{
	program_use(p);
	glUniform3fv(glGetUniformLocation(p->index, name), 1, v);
}

void shader_set_mat4(struct s_program *p, char const *name, mat4 m)
{
	program_use(p);
	glUniformMatrix4fv(glGetUniformLocation(p->index, name), 1,
		GL_FALSE, m[0]);
}
