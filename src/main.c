#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "scop.h"
#include "shader.h"
#include "object.h"
#include "obj.h"
#include "bmp.h"

t_engine	g_engine;

int			shader_from_files(char const *vpath, char const *fpath,
	struct s_program *p)
{
	char	*vertex;
	char	*fragment;

	if (!read_file(vpath, &vertex) || !read_file(fpath, &fragment))
	{
		return (-1);
	}
	program_create(p, vertex, fragment);
	free(vertex);
	free(fragment);
	return (0);
}

int			main(int ac, char *av[])
{
	int			ret;
	GLFWwindow	*window;

	ret = 1;
	if (ac <= 1)
	{
		fprintf(stderr, "Usage: ./scop file.obj\n");
		return (1);
	}
	if (scop_glfw_init() < 0)
		return (1);
	engine_init(&g_engine);
	window = scop_init_window(g_engine.window.width, g_engine.window.height);
	if (NULL != window)
	{
		glViewport(0, 0, g_engine.window.width, g_engine.window.height);
		glEnable(GL_DEPTH_TEST);
		ret = scop(window, av[1]);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return (ret);
}

void		framebuffer_resize(GLFWwindow __unused *win, int width, int height)
{
	g_engine.window.width = width;
	g_engine.window.height = height;
	glViewport(0, 0, width, height);
}

void 		mouse_move(GLFWwindow __unused *win, double xpos, double ypos)
{
	t_mouse	*mouse;

	mouse = &g_engine.mouse;
	if (mouse->first_mouse) {
		mouse->last_x = xpos;
		mouse->last_y = ypos;
		mouse->first_mouse = 0;
	}

	float xoff = xpos - mouse->last_x;
	float yoff = mouse->last_y - ypos;

	mouse->last_x = xpos;
	mouse->last_y = ypos;

	xoff *= mouse->sensitivity * g_engine.delta_time;
	yoff *= mouse->sensitivity * g_engine.delta_time;

	mouse->yaw += xoff;
	mouse->pitch += yoff;

	if (mouse->pitch > 89.0f)
		mouse->pitch = 89.0f;
	if (mouse->pitch < -89.0f)
		mouse->pitch = -89.0f;

	vec3 front;
	glm_vec3_copy((vec3){
			cos(glm_rad(mouse->yaw)) * cos(glm_rad(mouse->pitch)),
			sin(glm_rad(mouse->pitch)),
			sin(glm_rad(mouse->yaw)) * cos(glm_rad(mouse->pitch)),
		}, front);

	glm_normalize(front);
	glm_vec3_copy(front, g_engine.camera.front);
}
