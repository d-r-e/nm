#include "../include/nm.h"

// static int get_text_sect(struct segment_command_64 seg, struct section_64 sect)
// {
// 	const unsigned char *ptr;
// 	int offset;
// 	int filetype;
// 	unsigned long long lines;

// 	ptr = (unsigned char*)(sect.offset + g_mach.mem + g_mach.fatarch.offset);
// 	offset= sect.offset;
// 	lines = sect.size / 16;
// 	if (sect.size % 16)
// 		lines++;
// 	filetype=g_mach.header.filetype - 1;
// 	if (filetype == 0)
// 		offset = seg.fileoff  - sect.offset;
// 	if (filetype > 1)
// 		filetype = 0;
// 	//printf("flags: %x\n", sect.flags);
// 	for (unsigned long long i = 0; i < lines; i++)
// 	{
// 		printf("0000000%d%0.8x\t",filetype,  offset);
// 		for (int n = 0; n < 16; ++n)
// 		{
// 			if (i * 16 + n == sect.size)
// 				break;
// 			printf("%.2x", *ptr);
// 			if (n < 16)
// 				printf(" ");
// 			ptr++;
// 		}
// 		printf("\n");
// 		offset += 16;
// 	}
// 	return(0);
// }


void print_section(struct section_64 sect)
{
	printf("0x%.16llx\n", sect.addr);
	printf("sect.offset %u \n", sect.offset);
	printf("sect.align %u \n", sect.align);
	printf("sect.flags %.8x \n", sect.flags);
}

static void save_section(struct section_64 section)
{
	struct section_64 *newsections;

	(void)section;
	g_mach.nsects++;

	newsections = malloc(sizeof(section) * g_mach.nsects);
	if (!newsections)
		exit (-1);
	for (int i = 0; i < g_mach.nsects - 1; ++i)
		ft_memcpy(&newsections[i], (void*)&g_mach.sections[i], sizeof(section));
	ft_memcpy(&newsections[g_mach.nsects - 1], (void*)&section, sizeof(section));
	if (g_mach.sections)
 		free(g_mach.sections);
	g_mach.sections = newsections;
}

static void save_section32(struct section section)
{
	struct section *newsections;

	(void)section;
	g_mach.nsects++;

	newsections = malloc(sizeof(section) * g_mach.nsects);
	if (!newsections)
		exit (-1);
	for (int i = 0; i < g_mach.nsects - 1; ++i)
		ft_memcpy(&newsections[i], (void*)&g_mach.sections32[i], sizeof(section));
	ft_memcpy(&newsections[g_mach.nsects - 1], (void*)&section, sizeof(section));
	if (g_mach.sections32)
 		free(g_mach.sections32);
	g_mach.sections32 = newsections;
}


int	parse_segment(const char *mem, struct segment_command_64 segment)
{
	unsigned const char			*ptr;
	struct section_64	section;

	if (segment.nsects < 0)
		return (-1);
	//printf("%d\n", segment.fileoff);
	ptr = (unsigned char*)(mem + sizeof(segment));
	for (uint32_t i = 0; i < segment.nsects; ++i)
	{
		ft_bzero(&section, sizeof(section));
		ft_memcpy(&section, ptr, sizeof(section));
		save_section(section);
		ptr += sizeof(section);
	}
	return (0);
}

int	parse_segment32(const char *mem, struct segment_command segment)
{
	unsigned char			*ptr;
	struct section	section;

	if (segment.nsects < 0)
		return (-1);
	//printf("%d\n", segment.fileoff);
	ptr = (unsigned char*)(mem + sizeof(segment));
	for (uint32_t i = 0; i < segment.nsects; ++i)
	{
		ft_bzero(&section, sizeof(section));
		ft_memcpy(&section, ptr, sizeof(section));
		save_section32(section);
		ptr += sizeof(section);
	}
	return (0);
	return (0);
}
