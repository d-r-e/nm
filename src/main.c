#include "../include/nm.h"

static int ft_nm(const char *path)
{	uint32_t magic;

	ft_bzero(&g_mach, sizeof(g_mach));
	g_mach.fd = open(path, O_RDONLY, NULL);
	if (g_mach.fd < 0)
		return(file_error(path, NO_SUCH_FILE));
	ft_bzero(&g_mach.s, sizeof(struct stat));
	if (fstat(g_mach.fd, &g_mach.s))
		return (close(g_mach.fd) & -1);
	if (! (0x8000 & g_mach.s.st_mode))
	{
		fprintf(stderr, "%s: %s: not a regular file.\n", BINARY, path);
		close(g_mach.fd);
		return (-1);
	}
    g_mach.mem = mmap(0, g_mach.s.st_size, PROT_READ, MAP_PRIVATE, g_mach.fd, 0);
	if (g_mach.mem == MAP_FAILED)
		return (strerr("Error: Not enough memory.\n"));
	magic = get_magic(g_mach.mem, &g_mach.s);
	if (magic == MH_MAGIC_64) {
		g_mach.header_size = sizeof(struct mach_header_64);
		ft_memcpy((void*)&g_mach.header, g_mach.mem, g_mach.header_size);
		printf("%s:\n", path);
		analyse_mach64();
	} else if (magic == MH_MAGIC)
		printf("32 bits mach-o binary\n");
	else if (magic == CAFEBABE){
		printf("Mach-O universal binary\n");
		read_fat();
	}
	else if (magic == FAT_MAGIC){
		printf("32 bits fat binary (cafebabe)\n");
		g_mach.header_size = sizeof(struct mach_header);
		ft_memcpy((void*)&g_mach.header32, g_mach.mem, sizeof(g_mach.header32));
		printf("%s:\n", path);
		ft_puts("HOLI");
		analyse_mach32();
	} else if (get_magic(g_mach.mem, &g_mach.s ) == FAT_MAGIC_64){
		printf("64 bits fat binary\n");
		read_fat();
	} else if (get_magic(g_mach.mem, &g_mach.s ) == ARCH_MAGIC) {
		printf("Arch file\n");
		read_arch();
	} else {
		fprintf(stderr, "%s: %s: file format not recognized.\n", BINARY, path);
	}
	munmap(g_mach.mem, g_mach.s.st_size);
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
		//system("leaks ft_otool");
	}
}
