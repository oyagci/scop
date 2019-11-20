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

int		scop(GLFWwindow *window, char const *filename)
{
	t_obj	obj;
	char *vertexShaderSource = NULL;
	char *fragmentShaderSource = NULL;
	struct s_program shader1;
	char *stoneWall;

	struct s_object o1;

	if (obj_load(&obj, filename) < 0) {
		fprintf(stderr, "obj_load: error\n");
		return (1);
	}
	obj_parse(&obj);
	obj_triangulate(&obj);

	if (!read_file("./shaders/vertex.glsl", &vertexShaderSource) ||
		!read_file("./shaders/fragment.glsl", &fragmentShaderSource)) {
		glfwTerminate();
		return (1);
	}

	program_create(&shader1, vertexShaderSource, fragmentShaderSource);

	free(vertexShaderSource);
	free(fragmentShaderSource);

	t_gltri *vdata = obj_get_triangles_arr(&obj);

	object_init(&o1, (float *)vdata, obj.triangles.size * 3 * 9,
		   sizeof(GLfloat) * 9,	&shader1);
	free(vdata);

	shader_set_vec3(&shader1, "lightColor", (vec3){ 1.0f, 1.0f, 1.0f });
	shader_set_vec3(&shader1, "lightPos", (vec3){ 5.0f, 1.0f, 5.0f });

	mat4 projection;
	glm_perspective(45.0f, (float)g_engine.window.width/(float)g_engine.window.height,
		0.1f, 100.0f, projection);

	shader_set_mat4(&shader1, "proj", projection);

	int width;
	int height;

	stoneWall = bmp_load("img/chaton_roux.bmp", &width, &height);

	if (!stoneWall) {
		fprintf(stderr, "Could not load texture\n");
		return (0);
	}

	GLuint	stoneWallTex;

	glGenTextures(1, &stoneWallTex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, stoneWallTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, stoneWall);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);

	free(stoneWall);

	shader_set_float(&shader1, "opacity", 0.0f);

	object_set_pos(&o1, g_engine.obj.pos);

	while (!glfwWindowShouldClose(window)) {

		update_delta_time();
		print_fps();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		process_input(window);

		// Rendering

		object_set_pos(&o1, g_engine.obj.pos);

		// Rotation
		object_roty(&o1, 10.f * g_engine.delta_time);
		object_rotx(&o1, g_engine.obj.rot[0]);
		object_roty(&o1, g_engine.obj.rot[1]);
		object_rotz(&o1, g_engine.obj.rot[2]);
		g_engine.obj.rot[0] = 0.0f;
		g_engine.obj.rot[1] = 0.0f;
		g_engine.obj.rot[2] = 0.0f;

		update_opacity(&o1);

		mat4 view;
		vec3 cam_target;

		glm_vec3_add(g_engine.camera.front, g_engine.camera.pos, cam_target);
		glm_lookat(g_engine.camera.pos, cam_target, g_engine.camera.up, view);
		shader_set_mat4(&shader1, "view", view);

		shader_set_vec3(&shader1, "viewPos", g_engine.camera.pos);

		object_draw(&o1);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	object_delete(&o1);
	program_delete(&shader1);

	return (0);
}

int	main(int __unused ac, char *av[])
{
	GLFWwindow	*window = NULL;

	if (ac <= 1) {
		fprintf(stderr, "Usage: ./scop file.obj\n");
		return (1);
	}

	engine_init(&g_engine);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(g_engine.window.width, g_engine.window.height, "scop", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Could not create window!\n");
		glfwTerminate();
		return (1);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_resize);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "Could not load GL loader\n");
		glfwTerminate();
		return (-1);
	}

	glViewport(0, 0, g_engine.window.width, g_engine.window.height);
	glEnable(GL_DEPTH_TEST);

	scop(window, av[1]);

	glfwDestroyWindow(window);
	glfwTerminate();

	return (0);
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
