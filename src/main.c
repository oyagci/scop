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
#include "stb_image.h"
#include "obj.h"
#include "bmp.h"

float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

t_engine	g_engine;

int		scop(GLFWwindow *window, char const *filename)
{
	t_obj	obj;
	char *vertexShaderSource = NULL;
	char *fragmentShaderSource = NULL;
	char *lampFragmentShaderSource = NULL;
	char *lampVertexShaderSource = NULL;
	struct s_program shader1;
	struct s_program lampShader;
	char *stoneWall;

	struct s_object o1;
	struct s_object lamp;

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

	if (!read_file("./shaders/vertex2.glsl", &lampVertexShaderSource) ||
		!read_file("./shaders/fragment2.glsl", &lampFragmentShaderSource)) {
		glfwTerminate();
		return (1);
	}

	program_create(&shader1, vertexShaderSource, fragmentShaderSource);
	program_create(&lampShader, lampVertexShaderSource, lampFragmentShaderSource);

	free(vertexShaderSource);
	free(fragmentShaderSource);
	free(lampFragmentShaderSource);
	free(lampVertexShaderSource);

	t_gltri *vdata = obj_get_triangles_arr(&obj);

	object_init(&o1, (float *)vdata, obj.triangles.size * 3 * 9,
		   sizeof(GLfloat) * 9,	&shader1);
	object_init(&lamp, cubeVertices, sizeof(cubeVertices) / sizeof(GLfloat),
		sizeof(GLfloat) * 6, &lampShader);

	free(vdata);

	shader_set_vec3(&shader1, "lightColor", (vec3){ 1.0f, 1.0f, 1.0f });

	mat4 projection;
	glm_perspective(45.0f, (float)g_engine.window.width/(float)g_engine.window.height,
		0.1f, 100.0f, projection);

	shader_set_mat4(&shader1, "proj", projection);
	shader_set_mat4(&lampShader, "proj", projection);

	int width;
	int height;
	int nchan;

	stoneWall = bmp_load("img/Stonewall15_512x512.bmp", &width, &height);

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

	while (!glfwWindowShouldClose(window)) {

		update_delta_time();
		print_fps();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window);

		// Rendering

		vec3 lightPos = { 5.0f, sin(glfwGetTime()) * 2.0f, 5.0f };

		shader_set_vec3(&shader1, "lightPos", lightPos);
		object_set_pos(&lamp, lightPos);

		mat4 view;
		vec3 cam_target;

		glm_vec3_add(g_engine.camera.front, g_engine.camera.pos, cam_target);
		glm_lookat(g_engine.camera.pos, cam_target, g_engine.camera.up, view);
		shader_set_mat4(&shader1, "view", view);
		shader_set_mat4(&lampShader, "view", view);

		shader_set_vec3(&shader1, "viewPos", g_engine.camera.pos);
		shader_set_float(&shader1, "opacity",
			glm_clamp(sin(glfwGetTime()) + 0.5f, 0.0f, 1.0f));

		object_roty(&o1, 10.f * g_engine.delta_time);

		object_set_scale(&lamp, 0.3f);

		object_draw(&lamp);
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
	glfwSetFramebufferSizeCallback(window, framebufferResize);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseMove);

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

void processInput(GLFWwindow *win)
{
	float speed = 4.f * g_engine.delta_time;

	if (glfwGetKey(win, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(win, GLFW_TRUE);
	}
	if (glfwGetKey(win, GLFW_KEY_W)) {
		vec3 newpos = {
			g_engine.camera.front[0] * speed,
			g_engine.camera.front[1] * speed,
			g_engine.camera.front[2] * speed
		};

		glm_vec3_add(g_engine.camera.pos, newpos, g_engine.camera.pos);
	}
	if (glfwGetKey(win, GLFW_KEY_S)) {
		vec3 newpos = {
			g_engine.camera.front[0] * -speed,
			g_engine.camera.front[1] * -speed,
			g_engine.camera.front[2] * -speed
		};

		glm_vec3_add(g_engine.camera.pos, newpos, g_engine.camera.pos);
	}
	if (glfwGetKey(win, GLFW_KEY_A)) {
		vec3 right;

		glm_vec3_cross(g_engine.camera.front, g_engine.camera.up, right);
		glm_normalize(right);
		glm_vec3_muladds(right, -speed, g_engine.camera.pos);
	}
	if (glfwGetKey(win, GLFW_KEY_D)) {
		vec3 right;

		glm_vec3_cross(g_engine.camera.front, g_engine.camera.up, right);
		glm_normalize(right);
		glm_vec3_muladds(right, speed, g_engine.camera.pos);
	}
	if (glfwGetKey(win, GLFW_KEY_SPACE)) {
		vec3 newpos = { 0.0f, 1.0f * speed, 0.0f };

		glm_vec3_add(g_engine.camera.pos, newpos, g_engine.camera.pos);
	}
	if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL)) {
		vec3 newpos = { 0.0f, 1.0f * -speed, 0.0f };

		glm_vec3_add(g_engine.camera.pos, newpos, g_engine.camera.pos);
	}
}

void framebufferResize(GLFWwindow __unused *win, int width, int height)
{
	g_engine.window.width = width;
	g_engine.window.height = height;
	glViewport(0, 0, width, height);
}

void mouseMove(GLFWwindow __unused *win, double xpos, double ypos)
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
