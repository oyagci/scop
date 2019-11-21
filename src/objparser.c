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

int				obj_load(t_obj *obj, char const *const filename)
{
	char *data;

	memset(obj, 0, sizeof(*obj));
	if (read_file(filename, &data) < 0)
	{
		return (-1);
	}
	obj->data = data;
	return (0);
}

enum e_obj_tok	obj_kind(char const *const stok)
{
	t_tuple_tok const	tokens[] = {
		{ "v ", OBJ_VERTEX },
		{ "vt ", OBJ_TEXTURE },
		{ "vn ", OBJ_NORMAL },
		{ "f ", OBJ_FACE },
	};
	size_t				i;

	i = 0;
	while (i < sizeof(tokens) / sizeof(*tokens))
	{
		if (!strncmp(stok, tokens[i].str, strlen(tokens[i].str)))
		{
			return (tokens[i].tok);
		}
		i += 1;
	}
	return (OBJ_NONE);
}

int				obj_add_vertice(t_vertex_container *vc, char const *v)
{
	t_vertex	vert;

	sscanf(v, "v %f %f %f %f", &vert.x, &vert.y, &vert.z, &vert.w);
	return (vertex_container_add(vc, &vert));
}

int				obj_add_data(t_obj *obj, char *line)
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

int				obj_parse(t_obj *obj)
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
