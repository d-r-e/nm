#include "../include/nm.h"

static int get_text_sect(struct section_64 sect)
{
	const unsigned char *ptr;
	int offset;
	unsigned long long lines;

	ptr = (unsigned char*)(sect.offset + g_mach.mem);
	printf("__text size: %llu\n", sect.size);
	offset= sect.offset;
	lines = sect.size / 16;
	if (sect.size % 16)
		lines++;
	printf("\tsize: %llu\n\tlines: %llu\n", sect.size, lines);
	for (unsigned long long i = 0; i < lines; i++)
	{
		printf("00000001%0.8x\t", offset);
		for (int n = 0; n < 16; ++n)
		{
			printf("%.2x", *ptr);
			if (n != 15)
				printf(" ");
			ptr++;
			if (i * 16 + n == sect.size - 1)
				break;
		}
		printf("\n");
		offset += 16;
	}
	return(0);
}

int	parse_segment(const char *mem, struct segment_command_64 segment)
{
	unsigned char			*ptr;
	struct section_64	section;

	if (segment.nsects < 0)
		return (-1);
	ptr = (unsigned char*)(mem + segment.fileoff + sizeof(segment));
	for (uint32_t i = 0; i < segment.nsects; ++i)
	{
		ft_bzero(&section, sizeof(section));
		ft_memcpy(&section, ptr, sizeof(section));
		if (!ft_strncmp(section.segname, "__TEXT", ft_strlen("__TEXT")))
		{
			if (!ft_strncmp(section.sectname, "__text", 6))
			{
				printf("Contents of (__TEXT,%.16s) section\n", section.sectname);
				get_text_sect(section);
			}
		}
		ptr += sizeof(section);
	}
	return (0);
}
