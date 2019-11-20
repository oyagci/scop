#include <stdlib.h>
#include "obj.h"
#include "common.h"

int	triangle_container_init(t_triangle_container *vc)
{
	vc = calloc_abort(500, sizeof(t_triangle));
	vc->capacity = 500;
	vc->size = 0;
	return (0);
}

int	triangle_container_add(t_triangle_container *vc, t_triangle *v)
{
	t_triangle	*d;

	if (vc->size >= vc->capacity)
	{
		d = realloc_abort(vc->data,
			sizeof(t_triangle) * (vc->capacity + 500));
		vc->data = d;
		vc->capacity += 500;
	}
	memcpy(vc->data + vc->size, v, sizeof(t_triangle));
	vc->size += 1;
	return (0);
}
