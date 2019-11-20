#include <stdio.h>
#include <stdlib.h>

void	*malloc_abort(size_t size)
{
	void	*p;

	p = malloc(size);
	if (!p)
	{
		perror("malloc");
		abort();
	}
	return (p);
}
