#include "../include/nm.h"

int is_elf(const char *memfile, struct stat *s)
{
	if (s->st_size < 4)
		return (FALSE);
	if (*memfile != 127)
	 	return (FALSE);
	if (ft_strncmp(memfile + 1, "ELF", 3))
		return (FALSE);
	return (TRUE);
}

int is_mach(const char *memfile, struct stat *s)
{
	int mach32 = MH_MAGIC;
	int mach64 = MH_MAGIC_64;

	if (s->st_size < 4)
		return (FALSE);
	if (!ft_memcmp((unsigned char*)&mach32, memfile, sizeof(mach32)))
		return (32);
	if (!ft_memcmp((unsigned char*)&mach64, memfile, sizeof(mach64)))
		return (64);
	return (FALSE);
}

int get_mach_header32(const char *memfile)
{
	(void)memfile;
	return(0);
}

struct mach_header_64 get_mach_header64(const char *memfile)
{
	struct mach_header_64 header;

	ft_memcpy((void*)&header, memfile, sizeof(header));
	for(int i = 0; i < 80; ++i)
		write(1, "-", 1);
	write(1, "\n", 1);
	printf("--Magic file: %x\n", header.magic);
	switch (header.filetype)
	{
		case 1:
			printf("--Type: %s\n", "object");
			break;
		case 2:
			printf("--Type: %s\n", "execut");
			break;
		case 3:
			printf("--Type: %s\n", "library");
			break;
		case 4:
			printf("--Type: %s\n", "core");
			break;
		default:
			break;
	}
	printf("--ncmds: %d\n", header.ncmds);
	for(int i = 0; i < 80; ++i)
		write(1, "-", 1);
	write(1, "\n", 1);
	return(header);
}

int analyse_elf(const char *s, const char *path)
{
	(void)path;
	printf("%s: %s: no symbols\n", BINARY, path);
	(void)s;
	return (0);
}
 