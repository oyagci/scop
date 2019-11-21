/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:32 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 16:09:39 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BMP_H
# define BMP_H

# include <stdint.h>

# define BI_UNCOMPRESSED	0
# define BI_RLE_8			1
# define BI_RLE_4			2
# define BI_BITFIELDS		3

# pragma pack(1)

typedef struct	s_bmp_img_hdr {
	uint32_t	bi_size;
	uint32_t	bi_width;
	uint32_t	bi_height;
	uint16_t	bi_planes;
	uint16_t	bi_bitcount;
	uint32_t	bi_compression;
	uint32_t	bi_size_image;
	uint32_t	bi_x_pixels_per_meter;
	uint32_t	bi_y_pixels_per_meter;
	uint32_t	bi_clr_used;
	uint32_t	bi_clr_important;
}				t_bmp_img_hdr;

typedef struct	s_bmp_hdr {
	char			bf_type[2];
	uint32_t		bf_size;
	uint16_t		bf_reserved1;
	uint16_t		bf_reserved2;
	uint32_t		bf_offset_bits;
	t_bmp_img_hdr	bi;
}				t_bmp_hdr;

# pragma pack()

void			*bmp_load(char const *path, int *width, int *height);
void			bmp_free(void *data);

#endif
