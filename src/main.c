#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "shader.h"
#include "object.h"
#include <math.h>
#include "stb_image.h"
#include <cglm/call.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define WIDTH		1280
#define HEIGHT		720
#define __unused	__attribute__((unused))

// Keep track of aspect ratio changes when window is resized.
// Set to `1' to force the first frame to set up the projections matrix.
int		g_update_projection = 1;
mat4	g_projection;

vec3	*g_cam_pos_p;
vec3	*g_cam_front_p;
vec3	*g_cam_up_p;

int		g_width = WIDTH;
int		g_height = HEIGHT;

GLfloat cubeVerts[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

void framebufferResize(GLFWwindow __unused *win, int width, int height)
{
	g_width = width;
	g_height = height;
	g_update_projection = 1;
	glViewport(0, 0, width, height);
}

double last_x = WIDTH / 2;
double last_y = HEIGHT / 2;

float pitch = 0;
float yaw = -90.0f;
int first_mouse = 1;
 
void mouseMove(GLFWwindow *win, double xpos, double ypos)
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
	glm_vec3_copy(front, *g_cam_front_p);
}

float g_delta_time = 0.0f;

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

int	read_file(char const *const filename, char **buf)
{
	int			fd = open(filename, O_RDONLY);
	struct stat	statbuf;

	if (fd < 0) {
		perror("fopen");
		return (0);
	}
	if (fstat(fd, &statbuf) == -1) {
		perror("fstat");
		return (0);
	}

	*buf = malloc(sizeof(char) * (statbuf.st_size + 1));

	int ret = read(fd, *buf, statbuf.st_size);

	// Could not read the file
	if (ret != statbuf.st_size) {
		free(buf);
		return (0);
	}

	// Null-terminate the string
	(*buf)[ret] = 0;

	// Don't forget to close the FD as we don't need it anymore
	close(fd);
	return (1);
}

int	main(void)
{
	GLFWwindow *window = NULL;

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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "Could not load GL loader\n");
		glfwTerminate();
		return (-1);
	}

	glfwSetFramebufferSizeCallback(window, framebufferResize);
	glfwSetCursorPosCallback(window, mouseMove);

	// XXX: Wrong inputs are given when running in VM for some reason...
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glViewport(0, 0, g_width, g_height);
	glEnable(GL_DEPTH_TEST);

	char *vertexShaderSource = NULL;
	char *fragmentShaderSource = NULL;
	char *lampFragmentShaderSource = NULL;

	if (!read_file("./shaders/vertex.glsl", &vertexShaderSource) ||
		!read_file("./shaders/fragment.glsl", &fragmentShaderSource) ||
		!read_file("./shaders/lamp_fragment.glsl", &lampFragmentShaderSource)) {
		glfwTerminate();
		return (1);
	}

	struct s_object		cube;
	struct s_object		lamp;
	struct s_program	cubeShader;
	struct s_program	lampShader;

	program_create(&cubeShader, vertexShaderSource, fragmentShaderSource);
	program_create(&lampShader, vertexShaderSource, lampFragmentShaderSource);

	object_init(&cube, cubeVerts,
		sizeof(cubeVerts) / sizeof(GLfloat), &cubeShader);

	vec3 lampPos = { 1.2f, 2.0f, -1.0f };
	object_init(&lamp, cubeVerts,
		sizeof(cubeVerts) / sizeof(GLfloat), &lampShader);

	object_set_pos(&lamp, lampPos);
	object_scale(&lamp, 0.2f);
	object_set_pos(&cube, (GLfloat []){ 0.0f, 0.0f, 0.0f });

	program_set_vec3(&cubeShader, "objectColor", (vec3){ 1.0f, 0.5f, 0.31f });
	program_set_vec3(&cubeShader, "lightColor", (vec3){ 1.0f, 1.0f, 1.0f });
	program_set_vec3(&cubeShader, "lightPos", lampPos);

	program_set_vec3(&cubeShader, "material.diffuse", (vec3){ 1.0f, 0.5f, 0.31f });
	program_set_vec3(&cubeShader, "material.specular", (vec3){ 0.5f, 0.5f, 0.5f });
	program_set_float(&cubeShader, "material.shininess", 32.0f);

	program_set_vec3(&cubeShader, "light.direction", (vec3){ -1.0f, -1.0f, -1.0f });
	program_set_vec3(&cubeShader, "light.ambient", (vec3){ 0.2f, 0.2f, 0.2f });
	program_set_vec3(&cubeShader, "light.diffuse", (vec3){ 0.5f, 0.5f, 0.5f });
	program_set_vec3(&cubeShader, "light.specular", (vec3){ 1.0f, 1.0f, 1.0f });

	/* CONTAINER TEXTURE */

	int w, h, nchan;
	unsigned char *containerTexData = stbi_load("img/container2.png",
		&w, &h, &nchan, 0);

	if (!containerTexData) {
		fprintf(stderr, "Texture not found.\n");
		return (0);
	}
	GLuint containerTexture;
	glGenTextures(1, &containerTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, containerTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		containerTexData);
	glGenerateMipmap(GL_TEXTURE_2D);
	program_set_int(&cubeShader, "material.diffuse", 0); // GL_TEXTURE0
	stbi_image_free(containerTexData);

	/* CONTAINER SPECULAR */

	containerTexData = stbi_load("img/container2_specular.png",
		&w, &h, &nchan, 0);

	if (!containerTexData) {
		fprintf(stderr, "Texture not found.\n");
		return (0);
	}
	GLuint containerSpecular;
	glGenTextures(1, &containerSpecular);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, containerSpecular);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		containerTexData);
	glGenerateMipmap(GL_TEXTURE_2D);
	program_set_int(&cubeShader, "material.specular", 1); // GL_TEXTURE0
	stbi_image_free(containerTexData);

	containerTexData = stbi_load("img/matrix.jpg", &w, &h, &nchan, 0);
	if (!containerTexData) {
		fprintf(stderr, "Texture not found.\n");
		return (0);
	}
	GLuint containerEmission;
	glGenTextures(1, &containerEmission);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, containerEmission);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, containerTexData);
	glGenerateMipmap(GL_TEXTURE_2D);
	//program_set_int(&cubeShader, "material.emission", 2);

	vec3 cam_front = { 0.0f, 0.0f, -1.0f };
	vec3 cam_up = { 0.0f, 1.0f, 0.0f };
	vec3 cam_pos = { 0.0f, 0.5f, 3.0f};

	g_cam_front_p = &cam_front;
	g_cam_pos_p = &cam_pos;
	g_cam_up_p = &cam_up;

	vec3 cubePositions[] = {
		{ 0.0f,  0.0f,  0.0f},
		{ 2.0f,  5.0f, -15.0f},
		{-1.5f, -2.2f, -2.5f},
		{-3.8f, -2.0f, -12.3f},
		{ 2.4f, -0.4f, -3.5f},
		{-1.7f,  3.0f, -7.5f},
		{ 1.3f, -2.0f, -2.5f},
		{ 1.5f,  2.0f, -2.5f},
		{ 1.5f,  0.2f, -1.5f},
		{-1.3f,  1.0f, -1.5f},
	};

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

			program_set_mat4(&cubeShader, "proj", g_projection);
			program_set_mat4(&lampShader, "proj", g_projection);

			g_update_projection = 0;
		}

		// Change Light's position over time
		float x = sin(glfwGetTime());
		float y = cos(glfwGetTime());

		mat4 view;

		vec3 cam_target;
		glm_vec3_add(cam_front, cam_pos, cam_target);

		glm_lookat(cam_pos, cam_target, cam_up, view);

		program_set_vec3(&cubeShader, "viewPos", cam_pos);

		program_set_mat4(&cubeShader, "view", view);
		program_set_mat4(&lampShader, "view", view);

		for (size_t i = 0; i < sizeof(cubePositions) / sizeof(*cubePositions); i++) {
			object_set_pos(&cube, cubePositions[i]);
			object_rotx(&cube, 20.0f * i);
			object_roty(&cube, 5.0f * i);
			object_rotz(&cube, 10.0f * i);
			object_draw(&cube);
		}

//		object_draw(&lamp);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	object_delete(&cube);
	object_delete(&lamp);

	program_delete(&cubeShader);
	program_delete(&lampShader);

	free(vertexShaderSource);
	free(fragmentShaderSource);

	glfwDestroyWindow(window);
	glfwTerminate();

	return (0);
}
