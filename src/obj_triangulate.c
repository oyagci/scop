#include "obj.h"
#include "common.h"

static void		vertex_copy(t_vertex *src, vec3 dst)
{
	dst[0] = src->x;
	dst[1] = src->y;
	dst[2] = src->z;
}

void			obj_triangulate_face(t_obj *obj, t_face *face)
{
	t_triangle	t1;
	t_triangle	t2;
	vec3		ab;
	vec3		ac;
	vec3		norm;

	vertex_copy(&obj->vertices.data[face->indices[0].vert], t1.vert[0]);
	vertex_copy(&obj->vertices.data[face->indices[1].vert], t1.vert[1]);
	vertex_copy(&obj->vertices.data[face->indices[2].vert], t1.vert[2]);
	glm_vec3_sub(t1.vert[1], t1.vert[0], ab);
	glm_vec3_sub(t1.vert[2], t1.vert[0], ac);
	glm_cross(ab, ac, norm);
	glm_normalize(norm);
	glm_vec3_copy(norm, t1.norm);
	triangle_container_add(&obj->triangles, &t1);
	if (face->nverts == 4)
	{
		vertex_copy(&obj->vertices.data[face->indices[0].vert], t2.vert[0]);
		vertex_copy(&obj->vertices.data[face->indices[2].vert], t2.vert[1]);
		vertex_copy(&obj->vertices.data[face->indices[3].vert], t2.vert[2]);
		memcpy(t2.norm, norm, sizeof(vec3));
		triangle_container_add(&obj->triangles, &t2);
	}
}

void			obj_triangulate(t_obj *obj)
{
	size_t	i;

	i = 0;
	while (i < obj->faces.size)
	{
		obj_triangulate_face(obj, &obj->faces.data[i]);
		i++;
	}
}

t_gltri			*obj_get_triangles_arr(t_obj *obj)
{
	t_gltri		*triangles;
	t_triangle	*tri;
	size_t		i;
	float		color_val;
	vec3		color;

	triangles = malloc_abort(sizeof(t_gltri) * (obj->triangles.size));
	i = 0;
	while (i < obj->triangles.size)
	{
		tri = obj->triangles.data + i;
		color_val = i % 2 ? 0.7f : 1.0f;
		memcpy(color, (vec3){ color_val, color_val, color_val }, sizeof(vec3));
		memcpy(triangles[i].data[0].v, tri->vert[0], sizeof(vec3));
		memcpy(triangles[i].data[1].v, tri->vert[1], sizeof(vec3));
		memcpy(triangles[i].data[2].v, tri->vert[2], sizeof(vec3));
		memcpy(triangles[i].data[0].n, tri->norm, sizeof(vec3));
		memcpy(triangles[i].data[1].n, tri->norm, sizeof(vec3));
		memcpy(triangles[i].data[2].n, tri->norm, sizeof(vec3));
		memcpy(triangles[i].data[0].color, color, sizeof(vec3));
		memcpy(triangles[i].data[1].color, color, sizeof(vec3));
		memcpy(triangles[i].data[2].color, color, sizeof(vec3));
		i += 1;
	}
	return (triangles);
}