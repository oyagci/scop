#ifndef COMMON_H
# define COMMON_H

# include <stddef.h>

void	*malloc_abort(size_t size);
void	*realloc_abort(void *ptr, size_t size);
void	*calloc_abort(size_t nelem, size_t size);

#endif
