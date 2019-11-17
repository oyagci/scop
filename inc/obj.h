#ifndef OBJ_H
# define OBJ_H

# include <string.h>
# include "libft.h"
# include <stddef.h>

enum			e_obj_tok
{
	OBJ_NONE = 0,
	OBJ_VERTEX,
	OBJ_TEXTURE,
	OBJ_NORMAL,
	OBJ_FACE,
};

typedef struct	s_vertex {
	float	x;
	float	y;
	float	z;
	float	w;
}				t_vertex;

typedef struct	s_obj
{
	char			*data;
	t_vertex		*vertices;
	size_t			nverts;

	struct s_list	*faces;

	t_list			*triangles;
	size_t			ntriangles;
}				t_obj;

typedef struct	s_face_indices {
	size_t	vert;
	size_t	text;
	size_t	norm;
}				t_face_indices;

typedef struct	s_face {
	t_face_indices	*indices;
	size_t			nverts;
}				t_face;

typedef struct	s_triangle {
	size_t	vert[3];
}				t_triangle;

int		obj_load(t_obj *obj, char const *const filename);
int		obj_parse(t_obj *obj);
unsigned int	*obj_get_indices(t_obj *obj);
float	*obj_get_vertices(t_obj *obj);
void	obj_triangulate(t_obj *obj);

#endif
