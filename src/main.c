#include "../include/nm.h"

static int ft_nm(const char *path)
{
	char *ptr = NULL;
	int status;
	struct mach_header_64 header;

	ft_bzero(&g_mach, sizeof(g_mach));
	g_mach.fd = open(path, O_RDONLY, NULL);
	//printf("errno %d\n", errno);
	if (errno == 13)
		return(permission_denied(path));
	if (errno == 2)
		return(no_such_file(path));
	ft_bzero(&g_mach.s, sizeof(struct stat));
	if (fstat(g_mach.fd, &g_mach.s))
		return (close(g_mach.fd) & -1);
	//printf("stmode: %d\n", s.st_mode);
	if (! (0x8000 & g_mach.s.st_mode))
	{
		fprintf(stderr, "%s: %s: not a regular file\n", BINARY, path);
		return (-1);
	}
	//printf("Status: %d\ng_mach.fd: %d\nErrno fstat: %d\n", status, g_mach.fd, errno);
	(void)status;
    ptr = mmap(0, g_mach.s.st_size, PROT_READ, MAP_PRIVATE, g_mach.fd, 0);
	if (is_elf(ptr, &g_mach.s) == TRUE)
		analyse_elf(ptr, path);
	else if (is_mach(ptr, &g_mach.s) == 64)
	{
		header = get_mach_header64(ptr);
	}
	else
		fprintf(stderr, "%s: %s: file format not recognized\n", BINARY, path);
	munmap(ptr, g_mach.s.st_size);
	close(g_mach.fd);
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
		//system("leaks ft_nm");
	}
}
