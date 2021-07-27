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

uint32_t get_magic(const char *memfile, struct stat *s)
{
	uint32_t magic[5] = {MH_MAGIC, MH_MAGIC_64, FAT_MAGIC, FAT_MAGIC_64, CAFEBABE};
	const char magicstr[] = {ARMAG};

	if (s->st_size < (off_t)sizeof(struct mach_header))
		return (FALSE);
	for (int i = 0; i < 5; ++i) {
		if (!ft_memcmp((unsigned char*)&magic[i], memfile, sizeof(magic[i])))
		 	return (magic[i]);
	}
	for (int i = 0; i < 1; ++i) {
		if (!ft_memcmp(&magicstr[i], memfile, strlen(&magicstr[i])))
			return ((uint32_t)ARCH_MAGIC);
	}
	printf("%x\n", *memfile);
	for (int i = 0; i < 5; ++i) {
		printf("%.2x ", (unsigned char)*(memfile++));
	}
	printf("\n");
	return (FALSE);
}

int get_mach_header32(const char *memfile)
{
	(void)memfile;
	return(0);
}

int read_symstr(const char *mem, uint32_t nsyms)
{
	if (nsyms < 0)
		return (-1);
	mem++;
	for (uint32_t i = 0; i < nsyms; ++i)
		ft_puts(get_symstr(i));
	return(0);
}

const char  *get_symstr(uint32_t index)
{
	const char	*ptr;
	size_t	j;
	ptr = g_mach.mem + g_mach.symtab.stroff;
	for (uint32_t i = 0; i < g_mach.symtab.nsyms; ++i)
	{
		j = ft_strlen(ptr);
		if (i == index)
		{
			//printf("sym%d: %s\n", i, ptr);
			if (ft_strlen(ptr))
				return (ptr);
		}
		ptr += j + 1;
	}
	return (NULL);
}

int read_symtable_64(const char *mem, uint32_t nsyms)
{
	struct nlist_64 table;
	const char *ptr;
	uint32_t i;
	int ntypes[5] = {0x0, 0x2, 0xe, 0xc, 0xa};

	if (!mem || nsyms < 0)
		return(-1);
	ptr = mem;
	for (i = 0; i < nsyms; i++)
	{
		ft_bzero(&table, sizeof(table));
		ft_memcpy(&table, ptr, sizeof(table));
		printf("N_STAB:\t%d\n", table.n_type & N_STAB);
		printf("N_PEXT:\t%d\n", table.n_type & N_PEXT);
		if (table.n_type & N_TYPE)
		{
			for (int n = 0; n < 5; n++)
				printf("N_TYPE:\t%d\n", table.n_type & N_TYPE & ntypes[n]);
		}
		printf("N_EXT:\t%d\n", table.n_type & N_EXT);
		printf("n_strx:\t%d\n", table.n_un.n_strx);
		ptr += sizeof(table);
	}
	//nlist("a.out", list);
	return (0);
}

struct mach_header_64 get_mach_header64(const char *memfile)
{
	ft_memcpy((void*)&g_mach.header, memfile, sizeof(g_mach.header));
	// for(int i = 0; i < 80; ++i)
	// 	write(1, "-", 1);
	// write(1, "\n", 1);
	// // printf("--Magic file: %x\n", g_mach.header.magic);
	// switch (g_mach.header.filetype)
	// {
	// 	case 1:
	// 		printf("--Type: %s\n", "object");
	// 		break;
	// 	case 2:
	// 		printf("--Type: %s\n", "execut");
	// 		break;
	// 	case 3:
	// 		printf("--Type: %s\n", "library");
	// 		break;
	// 	case 4:
	// 		printf("--Type: %s\n", "core");
	// 		break;
	// 	default:
	// 		break;
	// }
	// printf("--ncmds: %d\n", g_mach.header.ncmds);
	// for(int i = 0; i < 80; ++i)
	// 	write(1, "-", 1);
	// write(1, "\n", 1);
	return(g_mach.header);
}

int analyse_elf(const char *s, const char *path)
{
	(void)path;
	printf("%s: %s: no symbols\n", BINARY, path);
	(void)s;
	return (0);
}

int analyse_mach32(void)
{
	struct load_command *ptr;
	unsigned const char *mem;

	ft_puts("CARACOLI");
	ptr = (struct load_command *)((g_mach.mem) + sizeof(g_mach.header32));
	mem = (const unsigned char *)ptr;
	printf("ncmds=%u\n", g_mach.header32.ncmds);
	ft_puts("CARACOLI");
	for (uint32_t i = 0; i < g_mach.header32.ncmds; i++)
	{
		// if (ptr->cmd == LC_SEGMENT)
		// {
		// 	struct segment_command segment;
			
		// 	ft_memcpy(&segment, ptr, sizeof(struct segment_command));
		// 	parse_segment32((void*)ptr, segment);
		// }
		// else if (ptr->cmd == LC_SYMTAB)
		// {
		// 	ft_memcpy(&g_mach.symtab, ptr, sizeof(g_mach.symtab));
		// }
		// else if (ptr->cmd == LC_DYSYMTAB)
		// {
		// 	struct dysymtab_command cmd;
		// 	(void)cmd;
		// }
		mem += ptr->cmdsize;
		ptr = (struct load_command *)mem;
	}
	ft_puts("CARACOLI");
	return(0);
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
			struct segment_command_64 segment;
			
			ft_memcpy(&segment, ptr, sizeof(struct segment_command_64));
			// printf("LC_SEGMENT_64 %d\n", i);
			parse_segment((void*)ptr, segment);
			// printf("Load command %d\n", i);
			// printf("segname: %s\n", segment.segname);
			// printf("cmdsize %u\n", segment.cmdsize);
			// printf("vmaddr %llx\n", segment.vmaddr);
			// printf("nsects %d\n", segment.nsects);
			// for (int j = 0; j < segment.nsects; j++)
			// {
			// 	struct section_64 sect;

			// }
			// printf("--------------------------------\n");
		}
		else if (ptr->cmd == LC_SYMTAB)
		{
			ft_memcpy(&g_mach.symtab, ptr, sizeof(g_mach.symtab));
			//read_symstr((char*)g_mach.mem + g_mach.symtab.stroff, g_mach.symtab.nsyms);
			//read_symtable_64((char*)g_mach.mem + g_mach.symtab.symoff, g_mach.symtab.nsyms);
		}
		else if (ptr->cmd == LC_DYSYMTAB)
		{
			struct dysymtab_command cmd;
			(void)cmd;
			// ft_memcpy(&cmd, ptr, sizeof(cmd));
			// printf("LC_DYSYMTAB: to be continued...\n");
			// printf("ilocalsym: %d\n", cmd.ilocalsym);
			// printf("nlocalsym: %d\n", cmd.nlocalsym);
			// printf("cmdsize %u\n", cmd.cmdsize);
			// printf("tocoff: %d\n", cmd.tocoff);
			// printf("ntoc: %d\n", cmd.ntoc);
			// printf("--------------------------------\n");
		}
		mem += ptr->cmdsize;
		ptr = (struct load_command *)mem;
	}
	return(0);
}
 
