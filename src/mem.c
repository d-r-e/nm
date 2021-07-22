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
	ft_memcpy((void*)&g_mach.header, memfile, sizeof(g_mach.header));
	for(int i = 0; i < 80; ++i)
		write(1, "-", 1);
	write(1, "\n", 1);
	printf("--Magic file: %x\n", g_mach.header.magic);
	switch (g_mach.header.filetype)
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
	printf("--ncmds: %d\n", g_mach.header.ncmds);
	for(int i = 0; i < 80; ++i)
		write(1, "-", 1);
	write(1, "\n", 1);
	return(g_mach.header);
}

int analyse_elf(const char *s, const char *path)
{
	(void)path;
	printf("%s: %s: no symbols\n", BINARY, path);
	(void)s;
	return (0);
}

int	analyse_mach64(void)
{
	struct load_command *ptr;
	unsigned const char *mem;

	ptr = (struct load_command *)((g_mach.mem) + sizeof(g_mach.header));
	mem = (const unsigned char *)ptr;
	for (uint32_t i = 0; i < g_mach.header.ncmds; i++)
	{
		if (ptr->cmd == LC_SEGMENT_64)
		{
			struct segment_command_64 *segment = malloc(sizeof(struct segment_command_64));
			ft_memcpy(segment, ptr, sizeof(struct segment_command_64));
			printf("Load command %d\n", i);
			printf("segname: %s\n", segment->segname);
			printf("cmdsize %u\n", segment->cmdsize);
			printf("addr %llx\n", segment->vmaddr);
			printf("filesize %llu \n", segment->filesize);
			printf("fileoff %llu \n", segment->fileoff);
			printf("--------------------------------\n");
			free(segment);
		}
		else if (ptr->cmd == LC_SYMTAB)
		{
			struct symtab_command cmd;
			ft_memcpy(&cmd, ptr, sizeof(cmd));
			printf("LC_SYMTAB\n");
			printf("nsyms: %d\n", cmd.nsyms);
			printf("cmdsize %u\n", cmd.cmdsize);
			printf("symoff: %d\n", cmd.symoff);

		}
		mem += ptr->cmdsize;
		ptr = (struct load_command *)mem;
	}
	return(0);
}
 