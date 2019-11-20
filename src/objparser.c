#include <obj.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include <errno.h>
#include <assert.h>
#include "common.h"

int	read_file(char const *const filename, char **buf);

int	vertex_container_init(t_vertex_container *vc)
{
	vc = calloc(500, sizeof(t_vertex));
	if (vc) {
		vc->capacity = 500;
		vc->size = 0;
		return (0);
	}
	return (-1);
}

int	vertex_container_add(t_vertex_container *vc, t_vertex *v)
{
	t_vertex	*d;

	if (vc->size >= vc->capacity) {
		d = realloc(vc->data, sizeof(t_vertex) * (vc->capacity + 500));
		if (d) {
			vc->data = d;
			vc->capacity += 500;
		}
		else {
			return (-1);
		}
	}
	memcpy(vc->data + vc->size, v, sizeof(t_vertex));
	vc->size += 1;
	return (0);
}

int	face_container_init(t_face_container *vc)
{
	vc = calloc(500, sizeof(t_face));
	if (vc) {
		vc->capacity = 500;
		vc->size = 0;
		return (0);
	}
	return (-1);
}

int	face_container_add(t_face_container *vc, t_face *v)
{
	t_face	*d;

	if (vc->size >= vc->capacity) {
		d = realloc(vc->data, sizeof(t_face) * (vc->capacity + 500));
		if (d) {
			vc->data = d;
			vc->capacity += 500;
		}
		else {
			return (-1);
		}
	}
	memcpy(vc->data + vc->size, v, sizeof(t_face));
	vc->size += 1;
	return (0);
}

int	triangle_container_init(t_triangle_container *vc)
{
	vc = calloc(500, sizeof(t_triangle));
	if (vc) {
		vc->capacity = 500;
		vc->size = 0;
		return (0);
	}
	return (-1);
}

int	triangle_container_add(t_triangle_container *vc, t_triangle *v)
{
	t_triangle	*d;

	if (vc->size >= vc->capacity) {
		d = realloc(vc->data, sizeof(t_triangle) * (vc->capacity + 500));
		if (d) {
			vc->data = d;
			vc->capacity += 500;
		}
		else {
			return (-1);
		}
	}
	memcpy(vc->data + vc->size, v, sizeof(t_triangle));
	vc->size += 1;
	return (0);
}

void ft_dlstpush(t_dlist **lstp, t_dlist *elem)
{
	t_dlist	*lst;
	t_dlist	*d;

	lst = *lstp;
	if (lst)
	{
		d = lst;
		while (d && d->next && d->next != lst)
			d = d->next;
		elem->next = lst;
		elem->prev = d;
		d->next = elem;
		lst->prev = elem;
	}
	else
	{
		*lstp = elem;
		elem->next = 0;
		elem->prev = 0;
	}
}

int	obj_load(t_obj *obj, char const *const filename)
{
	char *data;

	memset(obj, 0, sizeof(*obj));

	if (!read_file(filename, &data)) {
		return (1);
	}
	obj->data = data;
	return (0);
}

enum e_obj_tok	obj_kind(char const *const stok)
{
	struct { char *str; enum e_obj_tok tok; } tokens[] = {
		{ "v ", OBJ_VERTEX },
		{ "vt ", OBJ_TEXTURE },
		{ "vn ", OBJ_NORMAL },
		{ "f ", OBJ_FACE },
	};
	size_t	i = 0;

	while (i < sizeof(tokens) / sizeof(*tokens)) {
		if (!strncmp(stok, tokens[i].str, strlen(tokens[i].str))) {
			return (tokens[i].tok);
		}
		i += 1;
	}
	return (OBJ_NONE);
}

int	obj_add_vertice(t_vertex_container *vc, char const *v)
{
	t_vertex	vert;

	sscanf(v, "v %f %f %f %f", &vert.x, &vert.y, &vert.z, &vert.w);
	return (vertex_container_add(vc, &vert));
}

int	scan_verts(unsigned int vert[3], char const *line)
{
	if (*line) {
		sscanf(line, "%u/%u/%u", &vert[0], &vert[1], &vert[2]);
		if (vert[0] > 0) {
			vert[0]--;
		}
		if (vert[1] > 0) {
			vert[1]--;
		}
		if (vert[2] > 0) {
			vert[2]--;
		}
		if (vert[0] != 0) {
			return (1);
		}
	}
	return (0);
}

int	obj_add_face(t_obj *obj, const char *data)
{
	t_face	face;
	size_t	nverts;
	unsigned int verts[4][3];

	memset(verts, 0, sizeof(verts));

	nverts = 0;
	while (nverts < 4) {
		scan_verts(verts[nverts], data);
		nverts++;
		data = strchr(data, ' ');
		if (!data) {
			break ;
		}
		data++;
	}

	face.indices = malloc(sizeof(*face.indices) * nverts);
	face.nverts = nverts;

	size_t i = 0;
	while (i < nverts) {
		face.indices[i].vert = verts[i][0];
		face.indices[i].norm = verts[i][1];
		face.indices[i].text = verts[i][2];
		i++;
	}
	face_container_add(&obj->faces, &face);
	return (0);
}

int obj_add_data(t_obj *obj, char *line)
{
	enum e_obj_tok	kind;

	kind = obj_kind(line);
	if (kind == OBJ_VERTEX)
	{
		obj_add_vertice(&obj->vertices, line);
	}
	else if (kind == OBJ_FACE)
	{
		line += 2;
		obj_add_face(obj, line);
	}
	return (0);
}

int	obj_parse(t_obj *obj)
{
	char	*lines;
	char	*nl;

	vertex_container_init(&obj->vertices);
	face_container_init(&obj->faces);
	lines = obj->data;
	while (*lines)
	{
		nl = strchr(lines, '\n');
		if (nl)
		{
			*nl = 0;
			obj_add_data(obj, lines);
			lines = nl + 1;
		}
	}
	return (0);
}

void	vertex_copy(t_vertex *src, vec3 dst)
{
	dst[0] = src->x;
	dst[1] = src->y;
	dst[2] = src->z;
}

void	obj_triangulate_face(t_obj *obj, t_face *face)
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

void	obj_triangulate(t_obj *obj)
{
	size_t	i;

	i = 0;
	while (i < obj->faces.size)
	{
		obj_triangulate_face(obj, &obj->faces.data[i]);
		i++;
	}
}

t_gltri	*obj_get_triangles_arr(t_obj *obj)
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

float	*obj_get_vertices(t_obj *obj)
{
	return (float *)obj->vertices.data;
}
