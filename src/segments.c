#include "../include/nm.h"

int	parse_segment(const char *mem, struct segment_command_64 segment)
{
	char			*ptr;
	struct section_64	section;

	if (segment.nsects < 0)
		return (-1);
	printf("nsects: %d\n", segment.nsects);
	printf("segment fileoff: %llu\n", segment.fileoff);
	ptr = (char*)(mem + segment.fileoff + sizeof(segment));

	for (uint32_t i = 0; i < segment.nsects; ++i)
	{
		printf("section %d\n", i);
		ft_bzero(&section, sizeof(section));
		ft_memcpy(&section, ptr, sizeof(section));
		printf("section size %llu\n", section.size);
		write(1, section.sectname, 16);
		ptr += sizeof(section);
	}
	return (0);

}
