#include "glm.h"

void glm_lookat(vec3 eye,
	vec3 center,
	vec3 up,
	mat4 dest)
{
	vec3 f;
	vec3 u;
	vec3 s;

	glm_vec3_sub(center, eye, f);
	glm_vec3_normalize(f);
	glm_vec3_crossn(f, up, s);
	glm_vec3_cross(s, f, u);
	dest[0][0] = s[0];
	dest[0][1] = u[0];
	dest[0][2] =-f[0];
	dest[1][0] = s[1];
	dest[1][1] = u[1];
	dest[1][2] =-f[1];
	dest[2][0] = s[2];
	dest[2][1] = u[2];
	dest[2][2] =-f[2];
	dest[3][0] =-glm_vec3_dot(s, eye);
	dest[3][1] =-glm_vec3_dot(u, eye);
	dest[3][2] = glm_vec3_dot(f, eye);
	dest[0][3] = dest[1][3] = dest[2][3] = 0.0f;
	dest[3][3] = 1.0f;
}
