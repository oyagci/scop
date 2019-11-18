#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "shader.h"
#include "object.h"
#include <math.h>
#include "stb_image.h"
#include <cglm/call.h>
#include "obj.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define WIDTH		800
#define HEIGHT		600
#define __unused	__attribute__((unused))

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

void	processInput(GLFWwindow *win);
void	framebufferResize(GLFWwindow __unused *win, int width, int height);
int		read_file(char const *const filename, char **buf);
void	mouseMove(GLFWwindow __unused *win, double xpos, double ypos);

// Keep track of aspect ratio changes when window is resized.
// Set to `1' to force the first frame to set up the projections matrix.
int		g_update_projection = 1;
mat4	g_projection;

vec3	*g_cam_pos_p;
vec3	*g_cam_front_p;
vec3	*g_cam_up_p;

int		g_width = WIDTH;
int		g_height = HEIGHT;

double	last_x = WIDTH / 2;
double	last_y = HEIGHT / 2;

float	pitch = 0;
float	yaw = -90.0f;
int		first_mouse = 1;
 
float g_delta_time = 0.0f;

int		scop(GLFWwindow *window, char const *filename)
{
	t_obj	obj;
	char *vertexShaderSource = NULL;
	char *fragmentShaderSource = NULL;
	char *lampFragmentShaderSource = NULL;
	char *lampVertexShaderSource = NULL;
	struct s_program shader1;
	struct s_program lampShader;

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

	t_gltri *vertices1 = obj_get_triangles_arr(&obj);

	object_init(&o1, (float *)vertices1, NULL, obj.ntriangles * 3 * 6, 0, &shader1);

	object_init(&lamp, cubeVertices, NULL, sizeof(cubeVertices) / sizeof(GLfloat),
		0, &lampShader);
	object_set_scale(&lamp, 0.3f);
	object_set_pos(&o1, (GLfloat []){ 0.0f, 0.0f, 0.0f });

	shader_set_vec3(&shader1, "objectColor", (vec3){ 1.0f, 1.0f, 1.0f});
	shader_set_vec3(&shader1, "lightColor", (vec3){ 1.0f, 1.0f, 1.0f });

	free(vertices1);

	vec3 cam_front = { 0.0f, 0.0f, -1.0f };
	vec3 cam_up = { 0.0f, 1.0f, 0.0f };
	vec3 cam_pos = { 0.0f, 0.0f, 6.0f};

	g_cam_front_p = &cam_front;
	g_cam_pos_p = &cam_pos;
	g_cam_up_p = &cam_up;

	float delta_time = 0.0f;
	float last_frame = 0.0f;
	float total_time = 0.0f;
	size_t nimg = 0;
	while (!glfwWindowShouldClose(window)) {

		float current_frame = glfwGetTime();

		delta_time = current_frame - last_frame;
		last_frame = current_frame;
		g_delta_time = delta_time;

		// FPS Counter
		total_time += delta_time;
		nimg += 1;
		if (total_time >= 1.0f) {
			fprintf(stderr, "\rFPS: %d", (int)(nimg / total_time));
			total_time = 0.0f;
			nimg = 0;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window);

		// Rendering

		if (g_update_projection) {
			glm_perspective(45.0f, (float)g_width/(float)g_height, 0.1f, 100.0f,
				g_projection);

			shader_set_mat4(&shader1, "proj", g_projection);
			shader_set_mat4(&lampShader, "proj", g_projection);

			g_update_projection = 0;
		}

		mat4 view;
		vec3 cam_target;

		glm_vec3_add(cam_front, cam_pos, cam_target);
		glm_lookat(cam_pos, cam_target, cam_up, view);

		shader_set_mat4(&shader1, "view", view);
		shader_set_mat4(&lampShader, "view", view);

		object_roty(&o1, delta_time * glfwGetTime());
		object_draw(&o1);

		vec3 lightPos = { sin(glfwGetTime()) * 3.5f, 1.0f, 3.5f };

		shader_set_vec3(&shader1, "lightPos", lightPos);
		object_set_pos(&lamp, lightPos);
		object_draw(&lamp);

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

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(g_width, g_height, "scop", NULL, NULL);
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

	glViewport(0, 0, g_width, g_height);
	glEnable(GL_DEPTH_TEST);

	scop(window, av[1]);

	glfwDestroyWindow(window);
	glfwTerminate();

	return (0);
}

void processInput(GLFWwindow *win)
{
	float speed = 4.f * g_delta_time;

	if (glfwGetKey(win, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(win, GLFW_TRUE);
	}
	if (glfwGetKey(win, GLFW_KEY_W)) {
		vec3 newpos = {
			(*g_cam_front_p)[0] * speed,
			(*g_cam_front_p)[1] * speed,
			(*g_cam_front_p)[2] * speed
		};

		glm_vec3_add(*g_cam_pos_p, newpos, *g_cam_pos_p);
	}
	if (glfwGetKey(win, GLFW_KEY_S)) {
		vec3 newpos = {
			(*g_cam_front_p)[0] * -speed,
			(*g_cam_front_p)[1] * -speed,
			(*g_cam_front_p)[2] * -speed
		};

		glm_vec3_add(*g_cam_pos_p, newpos, *g_cam_pos_p);
	}
	if (glfwGetKey(win, GLFW_KEY_A)) {
		vec3 right;

		glm_vec3_cross(*g_cam_front_p, *g_cam_up_p, right);
		glm_normalize(right);
		glm_vec3_muladds(right, -speed, *g_cam_pos_p);
	}
	if (glfwGetKey(win, GLFW_KEY_D)) {
		vec3 right;

		glm_vec3_cross(*g_cam_front_p, *g_cam_up_p, right);
		glm_normalize(right);
		glm_vec3_muladds(right, speed, *g_cam_pos_p);
	}
	if (glfwGetKey(win, GLFW_KEY_SPACE)) {
		vec3 newpos = { 0.0f, 1.0f * speed, 0.0f };

		glm_vec3_add(*g_cam_pos_p, newpos, *g_cam_pos_p);
	}
	if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL)) {
		vec3 newpos = { 0.0f, 1.0f * -speed, 0.0f };

		glm_vec3_add(*g_cam_pos_p, newpos, *g_cam_pos_p);
	}
}

void framebufferResize(GLFWwindow __unused *win, int width, int height)
{
	g_width = width;
	g_height = height;
	g_update_projection = 1;
	glViewport(0, 0, width, height);
}

void mouseMove(GLFWwindow __unused *win, double xpos, double ypos)
{
	if (first_mouse) {
		last_x = xpos;
		last_y = ypos;
		first_mouse = 0;
	}

	float xoff = xpos - last_x;
	float yoff = last_y - ypos;

	last_x = xpos;
	last_y = ypos;

	float sensitivity = 0.1f;
	xoff *= sensitivity;
	yoff *= sensitivity;

	yaw += xoff;
	pitch += yoff;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	vec3 front;
	glm_vec3_copy((vec3){
			cos(glm_rad(yaw)) * cos(glm_rad(pitch)),
			sin(glm_rad(pitch)),
			sin(glm_rad(yaw)) * cos(glm_rad(pitch)),
		}, front);

	glm_normalize(front);
//	glm_vec3_print(front, stderr);
	glm_vec3_copy(front, *g_cam_front_p);
}
