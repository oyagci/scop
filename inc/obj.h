#ifndef OBJ_H
# define OBJ_H

# include <string.h>
# include "libft.h"
# include <stddef.h>
# include <cglm/cglm.h>

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

typedef struct	s_vertex_container
{
	t_vertex	*data;
	size_t		size;
	size_t		capacity;
}				t_vertex_container;

typedef struct	s_face_indices {
	size_t	vert;
	size_t	text;
	size_t	norm;
}				t_face_indices;

typedef struct	s_face {
	t_face_indices	*indices;
	size_t			nverts;
}				t_face;

typedef struct	s_face_container
{
	t_face		*data;
	size_t		size;
	size_t		capacity;
}				t_face_container;

typedef struct	s_obj
{
	char				*data;

	t_vertex_container	vertices;
	t_face_container	faces;

	t_list				*triangles;
	size_t				ntriangles;
}				t_obj;

typedef struct	s_triangle {
	vec3	vert[3];
	vec3	norm;
}				t_triangle;


/*
** Special structures for OpenGL which expects data in an array of numbers
** For each triangle we need:
**  - A Vertex Data
**  - A Normal Direction Vector
**  - (Optional) Diffuse Texture coordinates
**  - And anything we need to pass to the shaders...
*/
typedef struct	s_glvert {
	float	v[3];
	float	n[3];
}				t_glvert;

typedef struct	s_gltri {
	t_glvert	data[3];
}				t_gltri;

int				obj_load(t_obj *obj, char const *const filename);
int				obj_parse(t_obj *obj);
unsigned int	*obj_get_indices(t_obj *obj);
float			*obj_get_vertices(t_obj *obj);
void			obj_triangulate(t_obj *obj);
t_gltri			*obj_get_triangles_arr(t_obj *obj);

#endif
