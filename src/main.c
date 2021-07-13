#include "../include/nm.h"



static int ft_nm(const char *path)
{
	int fd;
	struct stat s;
	int status;
	char *ptr = NULL;

	fd = open(path, O_RDONLY, NULL);
	if (errno == 13)
		return(permission_denied(path));
	if (errno == 2)
		return(no_such_file(path));
	status = fstat(fd, &s);
	(void)status;
	//printf("Status: %d\nFD: %d\nErrno fstat: %d\n", status, fd, errno);

    ptr = mmap (0, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	write(1, ptr, 4);
	puts("write");
	if (ft_strncmp((char*)ptr, ".ELF", 4))
	{
		printf("ft_nm: %s: not an ELF file", path);
	}
	munmap(ptr, s.st_size);
	close(fd);
	return(0);
}



int main(int argc, char **argv)
{
	if (argc == 1)
		ft_nm("a.out");
	else if (argc > 1)
	{
		for (int i = 1; i < argc; ++i)
			ft_nm(argv[i]);
	}
}