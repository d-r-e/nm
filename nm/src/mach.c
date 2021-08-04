#include "../include/nm.h"

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
	return (FALSE);
}

int get_mach_header32(const char *memfile)
{
	(void)memfile;
	return(0);
}

const char  *get_symstr(struct nlist_64 table)
{
	return (g_mach.mem +g_mach.fatarch.offset +g_mach.symtab.stroff + table.n_un.n_strx);
}

const char  *get_symstr_32(struct nlist table)
{
	return (g_mach.mem +g_mach.fatarch.offset +g_mach.symtab.stroff + table.n_un.n_strx);
}

static char get_symbol_char(struct nlist_64 table)
{
	char c = 'U';

	if (table.n_sect > g_mach.nsects)
		return (c);
	if (!ft_strncmp(g_mach.sections[table.n_sect - 1].sectname, "__text", 7))
		c = 'T';
	else if (!ft_strncmp(g_mach.sections[table.n_sect - 1].sectname, "__bss", 6))
		c = 'B';
	else if (!ft_strncmp(g_mach.sections[table.n_sect - 1].sectname, "__data", 7))
		c = 'D';
	else
		c = 'S';
	if (!(table.n_type & N_EXT))
		c -= 'A' - 'a';
	return(c);
}

static char get_symbol_char_32(struct nlist table)
{
	char c = 'U';

	if (table.n_sect > g_mach.nsects)
		return (c);
	if (!ft_strncmp(g_mach.sections32[table.n_sect - 1].sectname, "__text", 6))
		c = 'T';
	else if (!ft_strncmp(g_mach.sections32[table.n_sect - 1].sectname, "__bss", 5))
		c = 'B';
	else if (!ft_strncmp(g_mach.sections32[table.n_sect - 1].sectname, "__data", 6))
		c = 'D';
	else
		c = 'S';
	if (!(table.n_type & N_EXT))
		c -= 'A' - 'a';
	return(c);
}

// static void print_table(struct nlist_64 table)
// {
// 	printf("table.n_sect %u\n", table.n_sect);
// 	printf("table.n_type %x\n", table.n_type);
// 	printf("table.n_sect %u\n", table.n_sect);
// 	printf("table.n_sect %u\n", table.n_sect);
// 	printf("table.n_sect %u\n", table.n_sect);
// }

int read_symtable_64(const char *mem, uint32_t nsyms)
{
	struct nlist_64 table;
	const char *ptr;
	uint32_t i;
	char c = 'X';

	if (!mem || nsyms < 0)
		return(-1);
	ptr = mem;
	for (i = 0; i < nsyms; i++)
	{
		ft_bzero(&table, sizeof(table));
		ft_memcpy(&table, ptr, sizeof(table));
		if ((table.n_type & N_STAB))
		{
			ft_putstr("");
		}
		else
		{
			switch ((table.n_type & N_TYPE))
			{
				case (N_STAB):
					break;
				case (N_UNDF):
					c = 'U';
					if (table.n_value){
						printf("%.16llx C ", table.n_value);
					}else{
						printf("%17sU ","");
					}
					printf("%s\n", get_symstr(table));
					break;
				case (N_ABS):
					printf("%.16llx ", table.n_value);
					printf("A %s\n", get_symstr(table));
					break;
				case (N_SECT):
					c = get_symbol_char(table);
					printf("%.16llx ", table.n_value);
					
					printf("%c %s\n",c, get_symstr(table));
					break;
				// case (N_PBUD):
				// 	printf("%.16llx ", table.n_value);
				// 	printf("P %s\n", get_symstr(table));
				// 	break;
				case (N_INDR):
					printf("%17sI","");
					printf(" %s (indirect for %s)\n",get_symstr(table) ,get_symstr(table));
					break;
				default:
					break;
			}
		}
		// printf("N_EXT:\t%d\n", table.n_type & N_EXT);
		// printf("n_strx:\t%d\n", table.n_un.n_strx);
		ptr += sizeof(table);
	}
	
	//nlist("a.out", list);
	return (0);
}

int read_symtable_32(const char *mem, uint32_t nsyms)
{
	struct nlist table;
	const char *ptr;
	uint32_t i;
	char c = 'U';

	if (!mem || nsyms < 0)
		return(-1);
	ptr = mem;
	for (i = 0; i < nsyms; i++)
	{
		ft_bzero(&table, sizeof(table));
		ft_memcpy(&table, ptr, sizeof(table));
		// printf("N_STAB:\t%d\n", table.n_type & N_STAB);
			switch ((table.n_type & N_TYPE))
			{
				case (N_UNDF):
					c = 'U';
					if (table.n_value){
						printf("%.8x C ", table.n_value);
					}else{
						printf("%9sU ","");
					}
					printf("%s\n", get_symstr_32(table));
					break;
				case (N_ABS):
					printf("%.8x ", table.n_value);
					printf("A %s\n", get_symstr_32(table));
					break;
				case (N_SECT):
					c = get_symbol_char_32(table);
					printf("%.8x ", table.n_value);
					printf("%c %s\n",c, get_symstr_32(table));
					break;
				// case (N_PBUD):
				// 	printf("%.16llx ", table.n_value);
				// 	printf("P %s\n", get_symstr_32(table));
				// 	break;
				case (N_INDR):
					printf("%.8x ", table.n_value);
					printf("I %s\n", get_symstr_32(table));
					break;
				default:
					break;
			}
		// printf("N_EXT:\t%d\n", table.n_type & N_EXT);
		// printf("n_strx:\t%d\n", table.n_un.n_strx);
		ptr += sizeof(table);
	}
	
	//nlist("a.out", list);
	return (0);
}

struct mach_header_64 get_mach_header64(const char *memfile)
{
	ft_bzero(&g_mach.header, sizeof(g_mach.header));
	ft_memcpy((void*)&g_mach.header, memfile, sizeof(g_mach.header));
	return(g_mach.header);
}

int analyse_mach32(struct load_command *ptr)
{
	unsigned const char *mem;
	mem = (const unsigned char *)ptr;

	for (uint32_t i = 0; i < g_mach.header.ncmds; i++)
	{
		if (ptr->cmd == LC_SEGMENT)
		{
			struct segment_command segment;
			ft_bzero(&segment, sizeof(struct segment_command));
			ft_memcpy(&segment, ptr, sizeof(struct segment_command));
			parse_segment32((void*)ptr, segment);
		}
		if (ptr->cmd == LC_SYMTAB)
		{
			ft_memcpy(&g_mach.symtab, ptr, sizeof(g_mach.symtab));
			read_symtable_32((char*)g_mach.mem + g_mach.fatarch.offset + g_mach.symtab.symoff, g_mach.symtab.nsyms);
		}
		// else if (ptr->cmd == LC_DYSYMTAB)
		// {
		// 	struct dysymtab_command cmd;
		// 	ft_memcpy(&cmd, ptr, sizeof(cmd));
		// }
		mem += ptr->cmdsize;
		ptr = (struct load_command *)mem;
		if (g_mach.nsects)
		{
			free(g_mach.sections);
		} 
	}
	return(0);
}

int	analyse_mach64(struct load_command *ptr)
{
	unsigned const char *mem;

	mem = (const unsigned char *)ptr;
	for (uint32_t i = 0; i < g_mach.header.ncmds; i++)
	{
		if (ptr->cmd == LC_SEGMENT_64)
		{
			struct segment_command_64 segment;
			ft_bzero(&segment, sizeof(struct segment_command_64));
			ft_memcpy(&segment, ptr, sizeof(struct segment_command_64));
			// printf("LC_SEGMENT_64 %d\n", i);
			parse_segment((void*)ptr, segment);
		}
		
		if (ptr->cmd == LC_SYMTAB)
		{
			ft_memcpy(&g_mach.symtab, ptr, sizeof(g_mach.symtab));
			read_symtable_64((char*)g_mach.mem + g_mach.fatarch.offset + g_mach.symtab.symoff, g_mach.symtab.nsyms);
			break;
		}
		mem += ptr->cmdsize;
		ptr = (struct load_command *)mem;
		if (g_mach.nsects)
			(void)(g_mach.sections);
	}
	return(0);
}
 
