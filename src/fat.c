#include "../include/nm.h"

static void ft_swap_fat_header(struct fat_header *fat)
{
    fat->magic = ft_ltob(fat->magic);
    fat->nfat_arch = ft_ltob(fat->nfat_arch);
}

// static void ft_swap_fat_arch(struct fat_arch *arch)
// {
//     arch->cputype = ft_ltob(arch->cputype);
//     arch->cpusubtype = ft_ltob(arch->cpusubtype);
//     arch->offset = ft_ltob(arch->offset);
//     arch->size = ft_ltob(arch->size);
//     arch->align = ft_ltob(arch->align);
// }

static void print_fat_arch(struct fat_arch arch)
{
    printf("arch.cputype : %i\n", arch.cputype);
    printf("arch.cpusubtype : %d\n", arch.cpusubtype);
    printf("arch.offset : %u\n", arch.offset);
    printf("arch.size : %u\n", arch.size);
    printf("arch.align : %u\n", arch.align);
}

int read_fat()
{
    const char *ptr;
    struct fat_arch     fat;
    struct fat_arch_64  fat64;
    unsigned int        i = 0;

    ptr = (char*) g_mach.mem;
    if (g_mach.s.st_size < (off_t)sizeof(struct fat_header))
        return (-1);
    memcpy((void*)&g_mach.fatheader, ptr, sizeof(g_mach.fatheader));
    printf("sizeof fatheader%lu\n", sizeof(g_mach.fatheader));
    ft_swap_fat_header(&g_mach.fatheader);
    printf("magic %x\n", g_mach.fatheader.magic);
    printf("nfat_arch %d\n", g_mach.fatheader.nfat_arch);
    ptr += 8;
    for (i = 0; i < g_mach.fatheader.nfat_arch; ++i)
    {
        cpu_type_t type = ft_ltob(*ptr);
        printf("cputype: %i\n", type);
        if (type == CPU_ARCH_ABI64_32){
            ft_memcpy(&fat, ptr, sizeof(fat));
            print_fat_arch(fat);
            ptr += sizeof(fat);
        }
        else if (type == CPU_ARCH_ABI64){
            ft_memcpy(&fat64, ptr, sizeof(fat64));
            printf("x86_64!\n");
            ptr += sizeof(fat64);
        } else {
            printf("Weird architecture\n");
            return (-1);
        }
        
        
    }
    
    return(0);
    ft_puts("END_______________________");
    return(0);
}
