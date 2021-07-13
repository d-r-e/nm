#include "../include/nm.h"



int nm(const char *path)
{
	int fd;
	struct stat s;
	int status;

	fd = open(path, O_RDONLY, NULL);
	status = fstat(fd, &s);
	if (status < 0)
		return (permission_denied(path));
	if (fd < 0)
		return (no_such_file(path));
	close(fd);
	return(0);
}



int main(int argc, char **argv)
{
	if (argc == 1)
		nm("a.out");
	else if (argc > 1)
	{
		for (int i = 1; i < argc; ++i)
			nm(argv[i]);
	}
}