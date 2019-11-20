#include "scop.h"

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
