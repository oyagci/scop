/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <oyagci@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 14:55:32 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/21 14:55:39 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "common.h"

int	read_file(char const *const filename, char **buf)
{
	int			fd;
	struct stat	statbuf;
	int			ret;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		fprintf(stderr, "%s\n", filename);
		perror("fopen");
		return (-1);
	}
	if (fstat(fd, &statbuf) == -1)
	{
		fprintf(stderr, "%s\n", filename);
		perror("fstat");
		return (-1);
	}
	*buf = malloc_abort(sizeof(char) * (statbuf.st_size + 1));
	ret = read(fd, *buf, statbuf.st_size);
	(*buf)[ret] = 0;
	close(fd);
	return (statbuf.st_size);
}
