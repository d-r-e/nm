#include "../include/nm.h"

static void ft_swap_fat_header(struct fat_header *fat)
{
    fat->magic = ft_swapint(fat->magic);
    fat->nfat_arch = ft_swapint(fat->nfat_arch);
}

static void ft_swap_fat_arch(struct fat_arch *arch)
{
    arch->cputype = ft_swapint(arch->cputype);
    arch->cpusubtype = ft_swapint(arch->cpusubtype);
    arch->offset = ft_swapint(arch->offset);
    arch->size = ft_swapint(arch->size);
    arch->align = ft_swapint(arch->align);
}

// static void ft_swap_fat_arch_64(struct fat_arch_64 *arch)
// {
//     arch->cputype = ft_swapint(arch->cputype);
//     arch->cpusubtype = ft_swapint(arch->cpusubtype);
//     arch->offset = ft_swaplonglong(arch->offset);
//     arch->size = ft_swaplonglong(arch->size);
//     arch->align = ft_swapint(arch->align);
// }

// static void print_fat_arch(struct fat_arch arch)
// {
//     printf("X32header\n");
//     printf("arch.cputype : %i\n", arch.cputype);
//     printf("arch.cpusubtype : %u\n", arch.cpusubtype);
//     printf("arch.offset : %u\n", arch.offset);
//     printf("arch.size : %u\n", arch.size);
//     printf("arch.align : %u\n", arch.align);
// }
// static void print_fat_arch64(struct fat_arch_64 arch)
// {
//     printf("X64header\n");
//     printf("arch.cputype : %u\n", arch.cputype);
//     printf("arch.cpusubtype : %u\n", arch.cpusubtype);
//     printf("arch.offset : %llu\n", arch.offset);
//     printf("arch.size : %llu\n", arch.size);
//     printf("arch.align : %u\n", arch.align);
// }

int is_x86(const char *ptr)
{
    cpu_type_t i386 = ft_swapint(CPU_TYPE_X86);

    for(int i = 0; i < 4; ++i)
    {
        if (((const char)ptr[i] != *((const char*)&i386 + i)))
            return (FALSE);
    }
    return (TRUE);
}

int is_x86_64(const char *ptr)
{
    cpu_type_t i386 = ft_swapint(CPU_TYPE_X86_64);

    for(int i = 0; i < 4; ++i)
    {
        if (((const char)ptr[i] != *((const char*)&i386 + i)))
            return (0);
    }
    return (1);
}

int read_fat()
{
    const char *ptr;
    unsigned int        i = 0;

    ptr = (char*) g_mach.mem;
    if (g_mach.s.st_size < (off_t)sizeof(struct fat_header))
        return (-1);
    ft_bzero(&g_mach.fatheader, sizeof(g_mach.fatheader));
    ft_memcpy((void*)&g_mach.fatheader, ptr, sizeof(g_mach.fatheader));
    //printf("sizeof fatheader%lu\n", sizeof(g_mach.fatheader));
    //printf("magic %X\n", g_mach.fatheader.magic);
    ft_swap_fat_header(&g_mach.fatheader);
    //printf("nfat_arch %d\n", g_mach.fatheader.nfat_arch);
    ptr = (const char*)g_mach.mem + sizeof(g_mach.fatheader);
    for (i = 0; i < g_mach.fatheader.nfat_arch; ++i)
    {
            ft_bzero(&g_mach.fatarch, sizeof(g_mach.fatarch));
            ft_memcpy(&g_mach.fatarch, ptr, sizeof(g_mach.fatarch));
            ft_swap_fat_arch(&g_mach.fatarch);
            //printf("%i\n", g_mach.fatarch.cputype);
            
            if (g_mach.fatarch.cputype == 16777223){
                //puts("entra");
            	g_mach.header_size = sizeof(struct mach_header_64);
                ft_bzero(&g_mach.header, sizeof(g_mach.header));
                ft_memcpy((void*)&g_mach.header, g_mach.mem + g_mach.fatarch.offset, sizeof(g_mach.header));
                //printf("ncmds%d\n", g_mach.header.ncmds);
                // hgeprint_fat_arch(g_mach.fatarch);
                analyse_mach64((struct load_command*)(g_mach.mem + g_mach.fatarch.offset + sizeof(g_mach.header)));
                //printf("entra\n");
            }
            //print_fat_arch(fat);
            // printf("______________\n");
            ptr += sizeof(struct fat_arch);
            //ft_puts("_______________________");
        
    }
    return(0);
    return(0);
}
