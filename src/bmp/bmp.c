/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:32 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 14:55:38 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"
#include "bmp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

void	bmp_print_hdrs(t_bmp_hdr *hdr)
{
	fprintf(stderr, "t_bmp_hdr {\n"
		"  bf_type[2]:     %2s\n"
		"  bf_size:        %d\n"
		"  bf_reserved1:   %d\n"
		"  bf_reserved2:   %d\n"
		"  bf_offset_bits: %d\n"
		"}\n",
		hdr->bf_type, hdr->bf_size, hdr->bf_reserved1, hdr->bf_reserved2,
		hdr->bf_offset_bits
	);
	fprintf(stderr, "t_bmp_img_hdr {\n"
		"  bi_size:               %d\n"
		"  bi_width:              %d\n"
		"  bi_height:             %d\n"
		"  bi_planes:             %d\n"
		"  bi_bitcount:           %d\n"
		"  bi_compression:        %d\n"
		"  bi_size_image:         %d\n"
		"  bi_x_pixels_per_meter: %d\n"
		"  bi_y_pixels_per_meter: %d\n"
		"  bi_clr_used:           %d\n"
		"  bi_clr_important:      %d\n"
		"}\n",
		hdr->bi.bi_size, hdr->bi.bi_width, hdr->bi.bi_height, hdr->bi.bi_planes,
		hdr->bi.bi_bitcount, hdr->bi.bi_compression, hdr->bi.bi_size_image,
		hdr->bi.bi_x_pixels_per_meter, hdr->bi.bi_y_pixels_per_meter,
		hdr->bi.bi_clr_used, hdr->bi.bi_clr_important
	);
}

int		bmp_copy_data(t_bmp_hdr *bmp, char *dest, size_t maxsz)
{
	char	*bytes;
	size_t	i;
	size_t	j;
	size_t	z;

	bytes = (char *)bmp + bmp->bf_offset_bits;
	z = 0;
	i = 0;
	while (i < bmp->bi.bi_width)
	{
		j = 0;
		while (j < bmp->bi.bi_height)
		{
			if (z + 2 > maxsz)
				return (0);
			dest[z + 2] = *bytes++;
			dest[z + 1] = *bytes++;
			dest[z + 0] = *bytes++;
			z += 3;
			j += 1;
		}
		i += 1;
	}
	return (0);
}

void	*bmp_load(char const *path, int *width, int *height)
{
	t_bmp_hdr		*hdr;
	void			*rawdata;
	char			*data;
	ssize_t			size;

	size = read_file(path, (char **)&rawdata);
	if (size < 0)
		return (NULL);
	hdr = rawdata;
	if (hdr->bf_type[0] != 'B' || hdr->bf_type[1] != 'M'
		|| hdr->bf_offset_bits > size)
	{
		free(rawdata);
		return (NULL);
	}
	*width = hdr->bi.bi_width;
	*height = hdr->bi.bi_height;
	data = malloc_abort(3 * hdr->bi.bi_width * hdr->bi.bi_height);
	bmp_copy_data(hdr, data, size - hdr->bf_offset_bits);
	free(rawdata);
	return (data);
}

void	bmp_free(void *data)
{
	free(data);
}
