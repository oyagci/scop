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

void	*realloc_abort(void *ptr, size_t size)
{
	void	*p;

	p = realloc(ptr, size);
	if (!p)
	{
		perror("realloc");
		abort();
	}
	return (p);
}

void	*calloc_abort(size_t nelem, size_t size)
{
	void	*p;

	p = calloc(nelem, size);
	if (!p)
	{
		perror("calloc");
		abort();
	}
	return (p);
}
