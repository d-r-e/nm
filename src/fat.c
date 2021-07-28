#include "../include/nm.h"

int read_fat()
{
    const char          *ptr;
    struct fat_arch_64  fat64;
    struct fat_arch     fat;
    unsigned int        i = 0;
    ptr = g_mach.mem;
    if (g_mach.s.st_size < (off_t)sizeof(struct fat_header))
        return (-1);
    printf("sizeof fatheader%lu\n", sizeof(g_mach.fatheader));
    ft_memcpy((void*)&g_mach.fatheader, ptr, sizeof(g_mach.fatheader));
    printf("magic %u\n", g_mach.fatheader.magic);
    printf("nfat_arch %d\n", (int)g_mach.fatheader.nfat_arch);
    ptr = g_mach.mem + sizeof(g_mach.fatheader);
    for (i = 0; i < ft_ltob(g_mach.fatheader.nfat_arch); ++i)
    {
        ft_bzero(&fat, sizeof(fat));
        ft_memcpy(&fat, ptr, sizeof(fat));
        ft_memcpy(&fat64, ptr, sizeof(fat64));
        printf("%d arch\n", (cpu_type_t)*ptr);
        ptr+= sizeof(fat);
    }
    return(0);
}
