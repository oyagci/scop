#include <stdio.h>
#include <string.h>
#include "shader.h"

void	program_init(struct s_program *p)
{
	memset(p, 0, sizeof(*p));
	p->index = glCreateProgram();
}

void	program_delete(struct s_program *p)
{
	glDeleteProgram(p->index);
}

void	program_use(struct s_program *p)
{
	glUseProgram(p->index);
}

void	program_create(struct s_program *p,
	const char *const vertsrc,
	const char *const fragsrc)
{
	char log[512];

	program_init(p);
	shader_init(&p->vertex, GL_VERTEX_SHADER);
	shader_init(&p->fragment, GL_FRAGMENT_SHADER);
	shader_load_source(&p->vertex, vertsrc);
	shader_compile(&p->vertex);
	if (!shader_compile_success(&p->vertex))
	{
		shader_infolog(&p->vertex, log, sizeof(log));
		fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", log);
	}
	shader_load_source(&p->fragment, fragsrc);
	shader_compile(&p->fragment);
	if (!shader_compile_success(&p->fragment))
	{
		shader_infolog(&p->fragment, log, sizeof(log));
		fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", log);
	}
	glAttachShader(p->index, p->vertex.index);
	glAttachShader(p->index, p->fragment.index);
	glLinkProgram(p->index);
	shader_delete(&p->vertex);
	shader_delete(&p->fragment);
}
