#include "../include/nm.h"

// int read_arch(void)
// {
//     struct ar_hdr arch;
//     const char *ptr;

//     ft_memcpy(&arch, g_mach.mem, sizeof(arch));
//     ptr = (const char *)&arch;
//     printf("%.16s\n", arch.ar_name);
//     ptr += sizeof(arch.ar_name);
//     return(0);
// }

size_t ft_strlen(const char *s)
{
    size_t i = 0;

    while (*s)
    {
        ++i;
        ++s;
    }
    return (i);
}