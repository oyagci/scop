#include "scop.h"
#include <stdio.h>

void print_fps(void)
{
	static float	total_time = 0.0f;
	static size_t	nimg = 0;

	total_time += g_engine.delta_time;
	nimg += 1;
	if (total_time >= 1.0f)
	{
		fprintf(stderr, "\rFPS: %d", (int)(nimg / total_time));
		total_time = 0.0f;
		nimg = 0;
	}
}
