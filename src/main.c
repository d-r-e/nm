#include "../include/nm.h"

static int ft_nm(const char *path)
{
	int fd;
	struct stat s;
	char *ptr = NULL;
	int status = 0;

	fd = open(path, O_RDONLY, NULL);
	if (errno == 13)
		return(permission_denied(path));
	if (errno == 2)
		return(no_such_file(path));
	ft_bzero(&s, sizeof(s));
	status = fstat(fd, &s);
	// //printf("Status: %d\nFD: %d\nErrno fstat: %d\n", status, fd, errno);
	(void)status;

    ptr = mmap(0, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (is_elf(ptr, &s) == TRUE)
		analyse(ptr);
	munmap(ptr, s.st_size);
	close(fd);
	return(0);
}

int main(int argc, char **argv)
{
	if (argc == 1)
		ft_nm("a.out");
	if (argc > 1)
	{
		for (int i = 1; i < argc; ++i)
			ft_nm(argv[i]);
	}
}