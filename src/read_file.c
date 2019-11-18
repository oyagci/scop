#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int	read_file(char const *const filename, char **buf)
{
	int			fd = open(filename, O_RDONLY);
	struct stat	statbuf;

	if (fd < 0) {
		fprintf(stderr, "%s\n", filename);
		perror("fopen");
		return (0);
	}
	if (fstat(fd, &statbuf) == -1) {
		perror("fstat");
		return (0);
	}

	*buf = malloc(sizeof(char) * (statbuf.st_size + 1));

	int ret = read(fd, *buf, statbuf.st_size);

	// Null-terminate the string
	(*buf)[ret] = 0;

	// Don't forget to close the FD as we don't need it anymore
	close(fd);
	return (1);
}
