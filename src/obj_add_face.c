#include <stdio.h>
#include "obj.h"
#include "common.h"

int				scan_verts(unsigned int vert[3], char const *line)
{
	if (*line)
	{
		sscanf(line, "%u/%u/%u", &vert[0], &vert[1], &vert[2]);
		if (vert[0] > 0)
			vert[0]--;
		if (vert[1] > 0)
			vert[1]--;
		if (vert[2] > 0)
			vert[2]--;
		if (vert[0] != 0)
			return (1);
	}
	return (0);
}

void			put_face_data(t_face_indices *inds, unsigned int vert[3])
{
	inds->vert = vert[0];
	inds->norm = vert[1];
	inds->text = vert[2];
}

int				obj_add_face(t_obj *obj, const char *data)
{
	t_face			face;
	size_t			nverts;
	unsigned int	verts[4][3];
	size_t			i;

	memset(verts, 0, sizeof(verts));
	nverts = 0;
	while (nverts < 4)
	{
		scan_verts(verts[nverts], data);
		nverts++;
		data = strchr(data, ' ');
		if (!data)
			break ;
		data++;
	}
	face.indices = malloc_abort(sizeof(*face.indices) * nverts);
	face.nverts = nverts;
	i = 0;
	while (i < nverts)
	{
		put_face_data(face.indices + i, verts[i]);
		i++;
	}
	face_container_add(&obj->faces, &face);
	return (0);
}
