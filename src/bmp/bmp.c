#include "scop.h"
#include "bmp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

int		bmp_copy_data(t_bmp_hdr *bmp, char **destp)
{
	size_t	z;
	char	*bytes;
	char	*dest;

	*destp = malloc(3 * bmp->bi.bi_width * bmp->bi.bi_height);
	dest = *destp;
	if (!*destp) {
		return (-1);
	}
	bytes = (char *)bmp + bmp->bf_offset_bits;
	z = 0;
	for (size_t i = 0; i < bmp->bi.bi_width; i++) {
		for (size_t j = 0; j < bmp->bi.bi_height; j++) {
			dest[z + 2] = *bytes++;
			dest[z + 1] = *bytes++;
			dest[z + 0] = *bytes++;
			z += 3;
		}
	}
	return (0);
}

void	*bmp_load(char const *path, int *width, int *height)
{
	t_bmp_hdr		*hdr;
	void			*rawdata;
	char			*data;

	if (read_file(path, (char **)&rawdata) < 0) {
		return (NULL);
	}
	hdr = rawdata;
	if (hdr->bf_type[0] != 'B' || hdr->bf_type[1] != 'M') {
		free(rawdata);
		return (NULL);
	}
	*width = hdr->bi.bi_width;
	*height = hdr->bi.bi_height;
	bmp_copy_data(hdr, &data);
	free(rawdata);
	return (data);
}

void	bmp_free(void *data)
{
	free(data);
}
