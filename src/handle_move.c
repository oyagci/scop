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
