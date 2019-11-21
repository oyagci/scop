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

void	update_camera(size_t n, struct s_program *shaders)
{
	mat4	view;
	vec3	cam_target;
	size_t	i;

	glm_vec3_add(g_engine.camera.front, g_engine.camera.pos, cam_target);
	glm_lookat(g_engine.camera.pos, cam_target, g_engine.camera.up, view);
	i = 0;
	while (i < n)
	{
		shader_set_mat4(shaders + i, "view", view);
		shader_set_vec3(shaders + i, "viewPos", g_engine.camera.pos);
		i += 1;
	}
}

void	update_opacity(struct s_object *o)
{
	float	speed;

	speed = 1.0f * g_engine.delta_time;
	if (g_engine.obj.opacity_dir > 0) {
		g_engine.obj.opacity_value += speed;
		shader_set_float(o->shader, "opacity", g_engine.obj.opacity_value);
		if (g_engine.obj.opacity_value >= 1.0f) {
			g_engine.obj.opacity_dir = 0;
			g_engine.obj.opacity_value = 1.0f;
		}
	}
	else if (g_engine.obj.opacity_dir < 0) {
		g_engine.obj.opacity_value -= speed;
		shader_set_float(o->shader, "opacity", g_engine.obj.opacity_value);
		if (g_engine.obj.opacity_value <= 0.05f) {
			g_engine.obj.opacity_dir = 0;
			g_engine.obj.opacity_value = 0.0f;
		}
	}
}

GLuint	texture_create(GLenum texture, char const *path)
{
	int		width;
	int		height;
	char	*texdata;
	GLuint	tex;

	texdata = bmp_load(path, &width, &height);
	if (!texdata)
	{
		fprintf(stderr, "Could not load texture\n");
		return (0);
	}
	glGenTextures(1, &tex);
	glActiveTexture(texture);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, texdata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);
	return (tex);
}

int		shader_from_files(char const *vpath, char const *fpath,
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

void	scop_shader_init(struct s_program *shader)
{
	mat4	projection;

	glm_perspective(45.0f,
		(float)g_engine.window.width / (float)g_engine.window.height,
		0.1f, 100.0f, projection);
	shader_set_vec3(shader, "lightColor", (vec3){ 1.0f, 1.0f, 1.0f });
	shader_set_vec3(shader, "lightPos", (vec3){ 5.0f, 1.0f, 5.0f });
	shader_set_float(shader, "opacity", 0.0f);
	shader_set_mat4(shader, "proj", projection);
}

void	scop_run(GLFWwindow *window, struct s_object *object)
{
	while (!glfwWindowShouldClose(window))
	{
		update_delta_time();
		print_fps();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		process_input(window);
		update_camera(1, object->shader);
		object_set_pos(object, g_engine.obj.pos);
		object_roty(object, 10.f * g_engine.delta_time);
		object_rotx(object, g_engine.obj.rot[0]);
		object_roty(object, g_engine.obj.rot[1]);
		object_rotz(object, g_engine.obj.rot[2]);
		g_engine.obj.rot[0] = 0.0f;
		g_engine.obj.rot[1] = 0.0f;
		g_engine.obj.rot[2] = 0.0f;
		update_opacity(object);
		object_draw(object);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

int		scop_obj_init(char const *objpath, t_obj *objbuf)
{
	if (obj_load(objbuf, objpath) < 0)
	{
		fprintf(stderr, "obj_load: error\n");
		return (-1);
	}
	obj_parse(objbuf);
	obj_triangulate(objbuf);
	return (0);
}

int		scop(GLFWwindow *window, char const *filename)
{
	struct s_program	shader;
	struct s_object		object;
	t_gltri				*vdata;
	GLuint				stonewall;
	t_obj				obj;

	scop_obj_init(filename, &obj);
	vdata = obj_get_triangles_arr(&obj);
	if (shader_from_files("./shaders/vertex.glsl", "./shaders/fragment.glsl",
		&shader) < 0)
		return (1);
	scop_shader_init(&shader);
	object_init(&object, (float *)vdata, obj.triangles.size * 3 * 9,
	   sizeof(GLfloat) * 9,	&shader);
	object_set_pos(&object, g_engine.obj.pos);
	free(vdata);
	stonewall = texture_create(GL_TEXTURE0, "img/chaton_roux.bmp");
	scop_run(window, &object);
	object_delete(&object);
	program_delete(&shader);
	glDeleteTextures(1, &stonewall);
	return (0);
}

int		scop_glfw_init(void)
{
	if (glfwInit() == GLFW_TRUE)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		return (0);
	}
	return (-1);
}

GLFWwindow *scop_init_window(int width, int height)
{
	GLFWwindow	*window;

	window = glfwCreateWindow(width, height, "scop", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Could not create window!\n");
		return (NULL);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_resize);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Could not load GL loader\n");
		glfwDestroyWindow(window);
		return (NULL);
	}
	return (window);
}

int	main(int ac, char *av[])
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

void framebuffer_resize(GLFWwindow __unused *win, int width, int height)
{
	g_engine.window.width = width;
	g_engine.window.height = height;
	glViewport(0, 0, width, height);
}

void mouse_move(GLFWwindow __unused *win, double xpos, double ypos)
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

void update_delta_time(void)
{
	static float	delta_time = 0.0f;
	static float	last_frame = 0.0f;
	float			current_frame;

	current_frame = glfwGetTime();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;
	g_engine.delta_time = delta_time;
}
