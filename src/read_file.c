#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	read_file(char const *const filename, char **buf)
{
	int			ret;
	int			fd;
	struct stat	statbuf;

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		perror("fopen");
		return (0);
	}
	if (fstat(fd, &statbuf) == -1) {
		perror("fstat");
		return (0);
	}
	*buf = malloc(sizeof(char) * (statbuf.st_size + 1));
	ret = read(fd, *buf, statbuf.st_size);
	(*buf)[ret] = 0;
	close(fd);
	return (1);
}
