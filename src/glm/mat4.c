#include "glm.h"
#include <math.h>

void	glm_mat4_copy(mat4 mat, mat4 dest)
{
	dest[0][0] = mat[0][0];  dest[1][0] = mat[1][0];
	dest[0][1] = mat[0][1];  dest[1][1] = mat[1][1];
	dest[0][2] = mat[0][2];  dest[1][2] = mat[1][2];
	dest[0][3] = mat[0][3];  dest[1][3] = mat[1][3];
	dest[2][0] = mat[2][0];  dest[3][0] = mat[3][0];
	dest[2][1] = mat[2][1];  dest[3][1] = mat[3][1];
	dest[2][2] = mat[2][2];  dest[3][2] = mat[3][2];
	dest[2][3] = mat[2][3];  dest[3][3] = mat[3][3];
}

void	glm_mat4_identity(mat4 mat)
{
	glm_mat4_copy((mat4){{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}, mat);
}

void	glm_mat4_zero(mat4 mat)
{
	glm_mat4_copy((mat4){{0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}}, mat);
}

void	glm_translate(mat4 m, vec3 v)
{
	vec4 v1;
	vec4 v2;
	vec4 v3;

	glm_vec4_scale(m[0], v[0], v1);
	glm_vec4_scale(m[1], v[1], v2);
	glm_vec4_scale(m[2], v[2], v3);
	glm_vec4_add(v1, m[3], m[3]);
	glm_vec4_add(v2, m[3], m[3]);
	glm_vec4_add(v3, m[3], m[3]);
}

void	glm_perspective(float fovy,
	float aspect,
	float nearVal,
	float farVal,
	mat4 dest)
{
	float f;
	float fn;

	glm_mat4_zero(dest);
	f  = 1.0f / tanf(fovy * 0.5f);
	fn = 1.0f / (nearVal - farVal);
	dest[0][0] = f / aspect;
	dest[1][1] = f;
	dest[2][2] = (nearVal + farVal) * fn;
	dest[2][3] = -1.0f;
	dest[3][2] = 2.0f * nearVal * farVal * fn;
}

void	glm_rotate_x(mat4 m, float angle, mat4 dest)
{
	mat4 t;
	float c, s;

	glm_mat4_identity(t);
	c = cosf(angle);
	s = sinf(angle);
	t[1][1] = c;
	t[1][2] = s;
	t[2][1] = -s;
	t[2][2] = c;
	glm_mul_rot(m, t, dest);
}

void	glm_rotate_y(mat4 m, float angle, mat4 dest)
{
	mat4 t;
	float c, s;

	glm_mat4_identity(t);
	c = cosf(angle);
	s = sinf(angle);
	t[0][0] = c;
	t[0][2] = -s;
	t[2][0] = s;
	t[2][2] = c;
	glm_mul_rot(m, t, dest);
}

void	glm_rotate_z(mat4 m, float angle, mat4 dest)
{
	mat4 t;
	float c, s;

	glm_mat4_identity(t);
	c = cosf(angle);
	s = sinf(angle);
	t[0][0] = c;
	t[0][1] = s;
	t[1][0] = -s;
	t[1][1] = c;
	glm_mul_rot(m, t, dest);
}

void	glm_mul_rot(mat4 m1, mat4 m2, mat4 dest)
{
	float const a[4][4] = {
		{ m1[0][0], m1[0][1], m1[0][2], m1[0][3] },
		{ m1[1][0], m1[1][1], m1[1][2], m1[1][3] },
		{ m1[2][0], m1[2][1], m1[2][2], m1[2][3] },
		{ m1[3][0], m1[3][1], m1[3][2], m1[3][3] }
	};
	float const b[3][3] = {
		{ m2[0][0], m2[0][1], m2[0][2] },
		{ m2[1][0], m2[1][1], m2[1][2] },
		{ m2[2][0], m2[2][1], m2[2][2] },
	};

	dest[0][0] = a[0][0] * b[0][0] + a[1][0] * b[0][1] + a[2][0] * b[0][2];
	dest[0][1] = a[0][1] * b[0][0] + a[1][1] * b[0][1] + a[2][1] * b[0][2];
	dest[0][2] = a[0][2] * b[0][0] + a[1][2] * b[0][1] + a[2][2] * b[0][2];
	dest[0][3] = a[0][3] * b[0][0] + a[1][3] * b[0][1] + a[2][3] * b[0][2];
	dest[1][0] = a[0][0] * b[1][0] + a[1][0] * b[1][1] + a[2][0] * b[1][2];
	dest[1][1] = a[0][1] * b[1][0] + a[1][1] * b[1][1] + a[2][1] * b[1][2];
	dest[1][2] = a[0][2] * b[1][0] + a[1][2] * b[1][1] + a[2][2] * b[1][2];
	dest[1][3] = a[0][3] * b[1][0] + a[1][3] * b[1][1] + a[2][3] * b[1][2];
	dest[2][0] = a[0][0] * b[2][0] + a[1][0] * b[2][1] + a[2][0] * b[2][2];
	dest[2][1] = a[0][1] * b[2][0] + a[1][1] * b[2][1] + a[2][1] * b[2][2];
	dest[2][2] = a[0][2] * b[2][0] + a[1][2] * b[2][1] + a[2][2] * b[2][2];
	dest[2][3] = a[0][3] * b[2][0] + a[1][3] * b[2][1] + a[2][3] * b[2][2];
	dest[3][0] = a[3][0];
	dest[3][1] = a[3][1];
	dest[3][2] = a[3][2];
	dest[3][3] = a[3][3];
}

void glm_scale_to(mat4 m, vec3 v, mat4 dest)
{
  glm_vec4_scale(m[0], v[0], dest[0]);
  glm_vec4_scale(m[1], v[1], dest[1]);
  glm_vec4_scale(m[2], v[2], dest[2]);

  glm_vec4_copy(m[3], dest[3]);
}

void	glm_scale_uni(mat4 m, float s)
{
  vec3 v = { s, s, s };

  glm_scale_to(m, v, m);
}
