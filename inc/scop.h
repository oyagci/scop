#ifndef SCOP_H
# define SCOP_H

# include "glad/glad.h"
# include <GLFW/glfw3.h>
# include <cglm/cglm.h>
# include <stdint.h>

# define WIDTH		800
# define HEIGHT		600
# define __unused	__attribute__((unused))

typedef struct	s_render_win {
	uint32_t	width;
	uint32_t	height;
}				t_render_win;

typedef struct	s_engine {
	vec3	cam_pos;
	vec3	cam_front;
	vec3	cam_up;
}				t_engine;

void	engine_init(t_engine *engine);

void	processInput(GLFWwindow *win);
void	framebufferResize(GLFWwindow __unused *win, int width, int height);
int		read_file(char const *const filename, char **buf);
void	mouseMove(GLFWwindow __unused *win, double xpos, double ypos);
void	print_fps(void);

#endif
