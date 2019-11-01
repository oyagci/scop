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

#define WIDTH		800
#define HEIGHT		600
#define __unused	__attribute__((unused))

GLfloat cube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

void framebufferResize(GLFWwindow __unused *win, int width, int height)
{
	glViewport(0, 0, width, height);
}

vec3 g_cam_pos;

void processInput(GLFWwindow *win)
{
	if (glfwGetKey(win, GLFW_KEY_W)) {
	}
	if (glfwGetKey(win, GLFW_KEY_S)) {
	}
	if (glfwGetKey(win, GLFW_KEY_A)) {
	}
	if (glfwGetKey(win, GLFW_KEY_D)) {
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "scop", NULL, NULL);
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
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);

	char *vertexShaderSource = NULL;
	char *fragmentShaderSource = NULL;

	if (!read_file("./shaders/vertex.glsl", &vertexShaderSource) ||
		!read_file("./shaders/fragment.glsl", &fragmentShaderSource)) {
		glfwTerminate();
		return (1);
	}

	struct s_program shader1;

	program_create(&shader1, vertexShaderSource, fragmentShaderSource);

//	GLfloat vertices1[] = {
//		// Vertices         // Colors         // Texture Coords
//		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
//		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
//		 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
//		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
//	};
	GLuint indices1[] = {
		0, 1, 2,
		0, 2, 3
	};

	struct s_object o1;

	object_init(&o1,
		cube, indices1,
		sizeof(cube) / sizeof(GLfloat),
		sizeof(indices1) / sizeof(GLuint),
		&shader1);

	// --- TEXTURE ---
	stbi_set_flip_vertically_on_load(1);

	int				w, h, n_chan;
	unsigned char	*tex_data = stbi_load("img/container.jpg",
		&w, &h, &n_chan, 0);

	if (!tex_data) {
		fprintf(stderr, "Could not load texture data\n");
		glfwTerminate();
		return (1);
	}

	GLuint texture;
	glGenTextures(1, &texture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
		tex_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(tex_data);

	tex_data = stbi_load("img/awesomeface.png", &w, &h, &n_chan, 0);
	if (!tex_data) {
		fprintf(stderr, "Could not load texture data\n");
		glfwTerminate();
		return (1);
	}

	GLuint texture2;
	glGenTextures(1, &texture2);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		tex_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	program_use(&shader1);

	glUniform1i(glGetUniformLocation(shader1.index, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shader1.index, "texture2"), 1);

	mat4 trans;
	mat4 model;
	mat4 view;
	mat4 persp;

	glm_mat4_identity(trans);
	glm_mat4_identity(model);
	glm_mat4_identity(view);
	glm_mat4_identity(persp);

	glm_rotate(model, glm_rad(-55.0f), (vec3){ 1.0f, 0.0f, 0.0f });
	glUniformMatrix4fv(glGetUniformLocation(shader1.index, "model"), 1,
		GL_FALSE, model[0]);

	glm_perspective(45.0f, (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f, persp);
	glUniformMatrix4fv(glGetUniformLocation(shader1.index, "proj"), 1,
		GL_FALSE, persp[0]);

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
		{-1.3f,  1.0f, -1.5f}  
	};

//	vec3 cam_pos = { 0.0f, 0.0f, -3.0f };
//	vec3 cam_target = { 0.0f, 0.0f, 0.0f };
//
//
//	vec3 cam_dir;
//	glm_vec3_sub(cam_pos, cam_target, cam_dir);
//
//
//	vec3 cam_right;
//	glm_cross((vec3){ 0.0f, 1.0f, 0.0f }, cam_dir, cam_right);
//	glm_normalize(cam_right);
//
//
//	vec3 cam_up;
//	glm_cross(cam_dir, cam_right, cam_up);
//
//	glm_lookat((vec3){ 0.0f, 0.0f, 3.0f }, (vec3){ 0.0f, 0.0f, 0.0f },
//		(vec3){ 0.0f, 1.0f, 0.0f }, view);

//	glUniformMatrix4fv(glGetUniformLocation(shader1.index, "view"), 1,
//		GL_FALSE, view[0]);

	vec3 cam_front = { 0.0f, 0.0f, -1.0f };
	vec3 cam_up = { 0.0f, 1.0f, 0.0f };
	vec3 cam_pos = { 0.0f, 0.0f, 3.0f};

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program_use(&shader1);

		vec3 cam_target;
		glm_vec3_add(cam_front, cam_pos, cam_target);

		glm_lookat(cam_pos, cam_target, cam_up, view);

		glUniformMatrix4fv(glGetUniformLocation(shader1.index, "view"), 1,
			GL_FALSE, view[0]);

		for (size_t i = 0; i < sizeof(cubePositions) / sizeof(*cubePositions); i++) {
			object_set_pos(&o1, cubePositions[i]);
			object_rotx(&o1, 20.0f * i);
			object_rotz(&o1, 20.0f * i);
			object_draw(&o1);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	object_delete(&o1);

	program_delete(&shader1);

	free(vertexShaderSource);
	free(fragmentShaderSource);

	glfwDestroyWindow(window);
	glfwTerminate();

	return (0);
}
