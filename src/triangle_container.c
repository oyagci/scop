#include <stdlib.h>
#include "obj.h"

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
