#include "../include/nm.h"

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
			printf("(__TEXT,%.16s)\n", section.sectname);
		ptr += sizeof(section);
	}
	return (0);

}
