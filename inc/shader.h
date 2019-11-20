#ifndef SHADER_H
# define SHADER_H

# include "glad/glad.h"
# include <GLFW/glfw3.h>
# include "glm.h"

struct s_shader {
    const char *source;
    GLuint index;
    GLenum type;
};

struct s_program
{
    struct s_shader	vertex;
    struct s_shader	fragment;
    GLuint index;
};

void	program_init(struct s_program *p);
void	program_create(struct s_program *p,
        const char *const vertsrc,
        const char *const fragsrc);
void 	program_use(struct s_program *p);
void	program_delete(struct s_program *p);
void	shader_set_vec3(struct s_program *p, char const *name, vec3 v);
void	shader_set_mat4(struct s_program *p, char const *name, mat4 m);
void	shader_set_float(struct s_program *p, char const *name, float f);

#endif
