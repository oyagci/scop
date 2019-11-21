#include <math.h>
#include "scop.h"

static void	mouse_camera_front(t_mouse *mouse)
{
	vec3	front;

	glm_vec3_copy((vec3){
			cos(glm_rad(mouse->yaw)) * cos(glm_rad(mouse->pitch)),
			sin(glm_rad(mouse->pitch)),
			sin(glm_rad(mouse->yaw)) * cos(glm_rad(mouse->pitch)),
		}, front);
	glm_normalize(front);
	glm_vec3_copy(front, g_engine.camera.front);
}

void 		mouse_move(GLFWwindow *win, double xpos, double ypos)
{
	float	xoff;
	float	yoff;
	t_mouse	*mouse;

	(void)win;
	mouse = &g_engine.mouse;
	if (mouse->first_mouse)
	{
		mouse->last_x = xpos;
		mouse->last_y = ypos;
		mouse->first_mouse = 0;
	}
	xoff = xpos - mouse->last_x;
	yoff = mouse->last_y - ypos;
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
	mouse_camera_front(mouse);
}
