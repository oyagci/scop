#include "scop.h"

void	handle_forward(float speed)
{
	glm_vec3_add(g_engine.obj.pos, (vec3){ 0.0f, 0.0f, speed },
		g_engine.obj.pos);
}

void	handle_up(float speed)
{
	glm_vec3_add(g_engine.obj.pos, (vec3){ 0.0f, speed, 0.0f },
		g_engine.obj.pos);
}

void	handle_left(float speed)
{
	glm_vec3_add(g_engine.obj.pos, (vec3){ speed, 0.0f, 0.0f },
		g_engine.obj.pos);
}

void	handle_pitch(float speed)
{
	glm_vec3_add(g_engine.obj.rot, (vec3){ speed, 0.0f, 0.0f },
		g_engine.obj.rot);
}

void	handle_yaw(float speed)
{
	glm_vec3_add(g_engine.obj.rot, (vec3){ 0.0f, speed, 0.0f  },
		g_engine.obj.rot);
}

void	handle_roll(float speed)
{
	glm_vec3_add(g_engine.obj.rot, (vec3){ 0.0f, 0.0f, speed },
		g_engine.obj.rot);
}

void	process_input(GLFWwindow *win)
{
	t_glfw_input const	keys[] = {
		{ GLFW_KEY_W, handle_forward, -1.0f },
		{ GLFW_KEY_S, handle_forward, 1.0f },
		{ GLFW_KEY_A, handle_left, -1.0f },
		{ GLFW_KEY_D, handle_left, 1.0f },
		{ GLFW_KEY_SPACE, handle_up, 1.0f },
		{ GLFW_KEY_LEFT_CONTROL, handle_up, -1.0f },
		{ GLFW_KEY_R, handle_pitch, 5.0f },
		{ GLFW_KEY_F, handle_pitch, -5.0f },
		{ GLFW_KEY_E, handle_yaw, 5.0f },
		{ GLFW_KEY_Q, handle_yaw, -5.0f },
		{ GLFW_KEY_X, handle_roll, 5.0f },
		{ GLFW_KEY_C, handle_roll, -5.0f },
	};
	float				speed = 4.f * g_engine.delta_time;
	size_t				i;

	i = 0;
	while (i < sizeof(keys) / sizeof(*keys))
	{
		if (glfwGetKey(win, keys[i].key)) {
			keys[i].handler(keys[i].dir * speed);
		}
		i += 1;
	}
	if (glfwGetKey(win, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(win, GLFW_TRUE);
	}
	if (glfwGetKey(win, GLFW_KEY_1)) {
		if (g_engine.obj.opacity_value >= 0.5f) {
			g_engine.obj.opacity_dir = -1;
		}
		else if (g_engine.obj.opacity_value < 0.5f) {
			g_engine.obj.opacity_dir = 1;
		}
		else {
			g_engine.obj.opacity_dir = 1;
		}

	}
}
