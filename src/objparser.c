#include <obj.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include <errno.h>
#include <assert.h>

int	read_file(char const *const filename, char **buf);

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

int	obj_add_vertice(t_list **vertlst, char const *v)
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;

	// Read values
	sscanf(v, "%f %f %f %f", &x, &y, &z, &w);

	t_list		*elem = ft_lstnew(NULL, 0);
	t_vertex	*vert = malloc(sizeof(t_vertex));

	vert->x = x;
	vert->y = y;
	vert->z = z;
	vert->w = w;

	elem->content = vert;
	ft_lstpush(vertlst, elem);

	return (0);
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
	t_face	*face;
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

	face = malloc(sizeof(*face));
	face->indices = malloc(sizeof(*face->indices) * nverts);
	face->nverts = nverts;

	size_t i = 0;
	while (i < nverts) {
		face->indices[i].vert = verts[i][0];
		face->indices[i].norm = verts[i][1];
		face->indices[i].text = verts[i][2];
		i++;
	}

	t_list	*elem = ft_lstnew(NULL, 0);
	elem->content = face;
	ft_lstpush(&obj->faces, elem);

	return (0);
}

t_vertex	*vertlst_to_arr(t_list **vertlstp, size_t nvert)
{
	t_vertex	*vertices;
	t_list		*vertlst;
	size_t		i;

	vertlst = *vertlstp;
	vertices = malloc(sizeof(*vertices) * nvert);
	i = 0;
	while (i < nvert) {
		memcpy(vertices + i, vertlst->content, sizeof(*vertices));
		vertlst = vertlst->next;
		i++;
	}
	return (vertices);
}

void	del_vertex(void *content,
	size_t __attribute__((unused)) csize)
{
	free(content);
}

int obj_add_data(t_obj *obj, char *line, t_list **vertlst)
{
	switch (obj_kind(line)) {
		case OBJ_VERTEX:
			line += 2;
			obj_add_vertice(vertlst, line);
			obj->nverts++;
			break ;
		case OBJ_FACE:
			line += 2;
			obj_add_face(obj, line);
			break ;
		default:
			break ;
	}
	return (0);
}

int	obj_parse(t_obj *obj)
{
	t_list	*vertlst;
	char	*lines;
	char	*nl;

	alarm(60);
	fprintf(stderr, "parsing");
	vertlst = NULL;
	lines = obj->data;
	while (*lines) {
		nl = strchr(lines, '\n');
		if (nl) {
			*nl = 0;
			obj_add_data(obj, lines, &vertlst);
			lines = nl + 1;
		}
	}
	fprintf(stderr, "done -- parsing");
	alarm(60);
	obj->vertices = vertlst_to_arr(&vertlst, obj->nverts);
	ft_lstdel(&vertlst, del_vertex);
	alarm(0);
	return (0);
}

void	obj_triangulate_face(t_obj *obj, t_face *face)
{
	t_list	*elem = NULL;
	t_list	*triangles;

	triangles = NULL;
	if (face->nverts == 4) {
		t_triangle *t1 = malloc(sizeof(*t1));
		t_triangle *t2 = malloc(sizeof(*t2));

		memcpy(&t1->vert[0], &obj->vertices[face->indices[0].vert], sizeof(float) * 3);
		memcpy(&t1->vert[1], &obj->vertices[face->indices[1].vert], sizeof(float) * 3);
		memcpy(&t1->vert[2], &obj->vertices[face->indices[2].vert], sizeof(float) * 3);

		memcpy(&t2->vert[0], &obj->vertices[face->indices[0].vert], sizeof(float) * 3);
		memcpy(&t2->vert[1], &obj->vertices[face->indices[2].vert], sizeof(float) * 3);
		memcpy(&t2->vert[2], &obj->vertices[face->indices[3].vert], sizeof(float) * 3);

		// Calculate the normal for the face

		vec3 *a = &t1->vert[0];
		vec3 *b = &t1->vert[1];
		vec3 *c = &t1->vert[2];

		vec3 ab;
		vec3 ac;
		vec3 norm;

		glm_vec3_sub(*b, *a, ab);
		glm_vec3_sub(*c, *a, ac);
		glm_cross(ab, ac, norm);
		glm_normalize(norm);

		memcpy(t1->norm, norm, sizeof(vec3));
		memcpy(t2->norm, norm, sizeof(vec3));

		elem = ft_lstnew(NULL, 0);
		elem->content = t1;
		ft_lstpush(&triangles, elem);

		elem = ft_lstnew(NULL, 0);
		elem->content = t2;
		ft_lstpush(&triangles, elem);

		obj->ntriangles += 2;
	}
	else if (face->nverts == 3) {
		t_triangle *t1 = malloc(sizeof(*t1));

		memcpy(&t1->vert[0], &obj->vertices[face->indices[0].vert], sizeof(float) * 3);
		memcpy(&t1->vert[1], &obj->vertices[face->indices[1].vert], sizeof(float) * 3);
		memcpy(&t1->vert[2], &obj->vertices[face->indices[2].vert], sizeof(float) * 3);

		// Calculate the normal for the face

		vec3 *a = &t1->vert[0];
		vec3 *b = &t1->vert[1];
		vec3 *c = &t1->vert[2];

		vec3 ab;
		vec3 ac;
		vec3 norm;

		glm_vec3_sub(*b, *a, ab);
		glm_vec3_sub(*c, *a, ac);
		glm_cross(ab, ac, norm);
		glm_normalize(norm);

		memcpy(t1->norm, norm, sizeof(vec3));

		elem = ft_lstnew(NULL, 0);
		elem->content = t1;
		ft_lstpush(&triangles, elem);

		obj->ntriangles += 1;
	}
	ft_lstpush(&obj->triangles, triangles);
}

void	obj_triangulate(t_obj *obj)
{
	t_list			*f = NULL;
	t_face			*face = NULL;
	
	fprintf(stderr, "Triangulating\n");
	f = obj->faces;
	while (f) {
		face = f->content;
		obj_triangulate_face(obj, face);
		f = f->next;
	}
}

t_gltri	*obj_get_triangles_arr(t_obj *obj)
{
	t_list	*t;
	t_gltri	*triangles;
	size_t	i;

	triangles = malloc(sizeof(t_gltri) * (obj->ntriangles));
	i = 0;
	t = obj->triangles;
	while (t) {
		t_triangle *content = t->content;

		memcpy(triangles[i].data[0].v, content->vert[0], sizeof(vec3));
		memcpy(triangles[i].data[1].v, content->vert[1], sizeof(vec3));
		memcpy(triangles[i].data[2].v, content->vert[2], sizeof(vec3));

		memcpy(triangles[i].data[0].n, content->norm, sizeof(vec3));
		memcpy(triangles[i].data[1].n, content->norm, sizeof(vec3));
		memcpy(triangles[i].data[2].n, content->norm, sizeof(vec3));

		i += 1;
		t = t->next;
	}

	return (triangles);
}

float	*obj_get_vertices(t_obj *obj)
{
	return (float *)obj->vertices;
}
