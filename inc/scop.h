#ifndef SCOP_H
# define SCOP_H

# include <GLFW/glfw3.h>

# define WIDTH		800
# define HEIGHT		600
# define __unused	__attribute__((unused))

void	processInput(GLFWwindow *win);
void	framebufferResize(GLFWwindow __unused *win, int width, int height);
int		read_file(char const *const filename, char **buf);
void	mouseMove(GLFWwindow __unused *win, double xpos, double ypos);
void	print_fps(void);

#endif
