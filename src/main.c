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

	if (read_file(vpath, &vertex) < 0 || read_file(fpath, &fragment) < 0)
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
		glfwDestroyWindow(window);
	}
	glfwTerminate();
	return (ret);
}

void		framebuffer_resize(GLFWwindow *win, int width, int height)
{
	(void)win;
	g_engine.window.width = width;
	g_engine.window.height = height;
	glViewport(0, 0, width, height);
}
