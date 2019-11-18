#include <obj.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include <errno.h>
#include <assert.h>

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
	t_list		*last;
	t_list		*lines;
	t_list		*elem;
	char		*data;
	int			fd;
	struct stat	st;

	alarm(10);
	fprintf(stderr, "Loading data in memory...\n");
	lines = NULL;
	memset(obj, 0, sizeof(*obj));
	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		return (-1);
	}
	if (-1 == fstat(fd, &st)) {
		perror("fstat");
		close(fd);
		return (-1);
	}
	if (!obj) {
		perror("malloc");
		return (-1);
	}

	if (get_next_line(fd, &data)) {
		elem = ft_lstnew(NULL, 0);
		elem->content = data;
		ft_lstpush(&lines, elem);
		last = elem;
	}
	int i = 1;
	while (get_next_line(fd, &data) > 0) {
		elem = ft_lstnew(NULL, 0);
		elem->content = data;
		ft_lstpush(&last, elem);
		last = last->next;
		fprintf(stdout, "%d\n", i++);
	}
	obj->data = lines;
	close(fd);
	fprintf(stderr, "Done -- Loading data in memory...\n");
	return (0);
}

enum e_obj_tok	obj_kind(char const *const stok)
{
	struct { char *str; enum e_obj_tok tok; } tokens[] = {
		{ "v", OBJ_VERTEX },
		{ "vt", OBJ_TEXTURE },
		{ "vn", OBJ_NORMAL },
		{ "f", OBJ_FACE },
	};
	size_t	i = 0;

	while (i < sizeof(tokens) / sizeof(*tokens)) {
		if (!strcmp(stok, tokens[i].str)) {
			return (tokens[i].tok);
		}
		i += 1;
	}
	return (OBJ_NONE);
}

int	obj_add_vertice(t_list **vertlst, char const **v)
{
	char const *sx = v[0];
	char const *sy = v[1];
	char const *sz = v[2];
	char const *sw = v[3];

	if (sx == 0 || sy == 0 || sz == 0) {
		// Invalid token. Missing x y or z component
		return (-1);
	}

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;

	// Read values
	sscanf(sx, "%f", &x);
	sscanf(sy, "%f", &y);
	sscanf(sz, "%f", &z);
	if (sw != 0) { sscanf(sw, "%f", &w); }

	t_list		*elem = ft_lstnew(NULL, 0);
	t_vertex	*vert = malloc(sizeof(t_vertex));

	vert->x = x;
	vert->y = y;
	vert->z = z;
	vert->w = w;

//	fprintf(stderr, "V { x: %.2f, y: %.2f, z: %.2f, w: %.2f }\n",
//		x, y, z, w);

	elem->content = vert;
	ft_lstpush(vertlst, elem);

	return (0);
}

int	obj_add_face(t_obj *obj, const char **fs)
{
	t_face	*face;
	size_t	nverts;
	size_t	i;

	nverts = 0;
	while (fs[nverts] != 0) {
		nverts++;
	}

	face = malloc(sizeof(*face));
	face->indices = malloc(sizeof(*face->indices) * nverts);
	face->nverts = nverts;

	i = 0;
	while (i < nverts) {
		size_t j = 0;

		// TODO: Refactor the stuff bellow. It's obviously bad.

		// Parse digit
		// index starts at 1 (I know...)
		face->indices[i].vert = ft_atoi(fs[i]) - 1;

		// Go to next digit
		while (fs[i][j] != '\0' && fs[i][j] != '/')
			j += 1;
		if (fs[i][j] == '/') {
			j += 1;
		}
		else {
			i += 1;
			continue ;
		}

		// Parse digit
		face->indices[i].text = ft_atoi(fs[i]) - 1;

		// Go to next digit
		while (fs[i][j] != '\0' && fs[i][j] != '/')
			j += 1;
		if (fs[i][j] == '/') {
			j += 1;
		}
		else {
			i += 1;
			continue ;
		}

		// Parse digit
		face->indices[i].norm = ft_atoi(fs[i]) - 1;

		i += 1;
	}

//	t_face_indices *indices = face->indices;
//	i = 0;
//	fprintf(stderr, "F {\n");
//	while (i < face->nverts) {
//		fprintf(stderr, "  { vert: %ld, norm: %ld, text: %ld }\n",
//			indices[i].vert, indices[i].norm, indices[i].text);
//		i++;
//	}
//	fprintf(stderr, "}\n");

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

int	obj_parse(t_obj *obj)
{
	t_list	*lines;
	char	**line;
	size_t	nvert;
	t_list	*vertlst;

	alarm(10);
	vertlst = 0;
	nvert = 0;
	lines = obj->data;
	int i = 0;
	while (lines) {
		line = ft_strsplit(lines->content, ' ');
		fprintf(stderr, "%d\n", i++);
		switch (obj_kind(line[0])) {
			case OBJ_VERTEX:
				obj_add_vertice(&vertlst, (const char **)(line + 1));
				nvert++;
				break ;
			case OBJ_FACE:
				obj_add_face(obj, (const char **)(line + 1));
				break ;
			default:
				break ;
		}
		lines = lines->next;
	}
	obj->vertices = vertlst_to_arr(&vertlst, nvert);
	obj->nverts = nvert;
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
//		fprintf(stderr, "T { %2lu, %2lu, %2lu }\n",
//				t1->vert[0], t1->vert[1], t1->vert[2]);

		obj->ntriangles += 1;
	}
	ft_lstpush(&obj->triangles, triangles);
}

void	obj_triangulate(t_obj *obj)
{
	t_list			*f = NULL;
	t_face			*face = NULL;
	
	fprintf(stderr, "Triangulating\n");
	alarm(10);
	f = obj->faces;
	while (f) {
		face = f->content;
		obj_triangulate_face(obj, face);
		f = f->next;
	}
	alarm(0);
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

unsigned int	*obj_get_indices(t_obj *obj)
{
	(void)obj;
	return (NULL);
//	t_list			*t;
//	t_triangle		*tri;
//	unsigned int	*indices = NULL;
//	size_t			i;
//
//	indices = malloc(sizeof(*indices) * (obj->ntriangles * 3));
//	t = obj->triangles;
//	i = 0;
//	while (i < obj->ntriangles) {
//		tri = t->content;
//
//		size_t	j;
//		j = 0;
//		while (j < 3) {
//			fprintf(stderr, "%lu %lu\n", i * 3, j);
//			indices[i * 3 + j] = tri->vert[j];
//			j += 1;
//		}
//
//		t = t->next;
//		i += 1;
//	}
//	return (indices);
}

float	*obj_get_vertices(t_obj *obj)
{
	return (float *)obj->vertices;
}
