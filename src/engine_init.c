#include <string.h>
#include "scop.h"

void	mouse_init(t_mouse *mouse)
{
	mouse->first_mouse = 1;
	mouse->last_x = WIDTH / 2;
	mouse->last_y = HEIGHT / 2;
	mouse->pitch = 0.0f;
	mouse->yaw = -90.0f;
	mouse->sensitivity = 2.6f;
}

void	render_win_init(t_render_win *window)
{
	window->width = WIDTH;
	window->height = HEIGHT;
}

void	gl_camera_init(t_gl_camera *camera)
{
	memcpy(&camera->front, (vec3){ 0.0f, 0.0f, -1.0f }, sizeof(vec3));
	memcpy(&camera->up, (vec3){ 0.0f, 1.0f, 0.0f }, sizeof(vec3));
	memcpy(&camera->pos, (vec3){ 0.0f, 0.0f, 6.0f }, sizeof(vec3));
}

void	engine_init(t_engine *engine)
{
	engine->delta_time = 0;
	mouse_init(&engine->mouse);
	render_win_init(&engine->window);
	gl_camera_init(&engine->camera);
}
