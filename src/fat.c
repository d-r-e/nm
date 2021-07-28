#include "../include/nm.h"

// static void ft_swap_fat_header(struct fat_header *fat)
// {
//     fat->magic = ft_ltob(fat->magic);
//     fat->nfat_arch = ft_ltob(fat->nfat_arch);
// }

static void ft_swap_fat_arch(struct fat_arch *arch)
{
    arch->cputype = ft_ltob(arch->cputype);
    arch->cpusubtype = ft_ltob(arch->cpusubtype);
    arch->offset = ft_ltob(arch->offset);
    arch->size = ft_ltob(arch->size);
    arch->align = ft_ltob(arch->align);
}

static void ft_swap_fat_arch_64(struct fat_arch_64 *arch)
{
    arch->cputype = ft_ltob(arch->cputype);
    arch->cpusubtype = ft_ltob(arch->cpusubtype);
    arch->offset = ft_lltob(arch->offset);
    arch->size = ft_lltob(arch->size);
    arch->align = ft_ltob(arch->align);
}

static void print_fat_arch(struct fat_arch arch)
{
    printf("arch.cputype : %i\n", arch.cputype);
    printf("arch.cpusubtype : %d\n", arch.cpusubtype);
    printf("arch.offset : %u\n", arch.offset);
    printf("arch.size : %u\n", arch.size);
    printf("arch.align : %u\n", arch.align);
}

static void print_fat_arch64(struct fat_arch_64 arch)
{
    printf("arch.cputype : %i\n", arch.cputype);
    printf("arch.cpusubtype : %d\n", arch.cpusubtype);
    printf("arch.offset : %llu\n", arch.offset);
    printf("arch.size : %llu\n", arch.size);
    printf("arch.align : %u\n", arch.align);
}

int is_x86(const char *ptr)
{
    int i386 = 0x07000000;

    for(int i = 0; i < 4; ++i)
    {
        if (((const char)ptr[i] != *((const char*)&i386 + i)))
            return (0);
    }
    return (1);
}

int is_x86_64(const char *ptr)
{
    int i386 = 0x07000001;

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
    struct fat_arch     fat;
    struct fat_arch_64  fat64;
    unsigned int        i = 0;

    ptr = (char*) g_mach.mem;
    if (g_mach.s.st_size < (off_t)sizeof(struct fat_header))
        return (-1);
    memcpy((void*)&g_mach.fatheader, ptr, sizeof(g_mach.fatheader));
    //printf("sizeof fatheader%lu\n", sizeof(g_mach.fatheader));
    //printf("magic %X\n", g_mach.fatheader.magic);
    swap_fat_header(&g_mach.fatheader, LITTLE_ENDIAN);
    //printf("nfat_arch %d\n", g_mach.fatheader.nfat_arch);
    ptr += sizeof(struct fat_header);
    for (i = 0; i < g_mach.fatheader.nfat_arch; ++i)
    {
        cpu_type_t type = *ptr;
        for(int i = 0; i < 4; ++i)
        {
            printf("%.2x ", (unsigned int)ptr[i]);
        }
        printf("cputype: %.8x\n", type);
        printf("CPU_TYPE_X86_64: %x\n", CPU_TYPE_X86_64);
        printf("CPU_TYPE_X86: %x\n", CPU_TYPE_X86);

        if (is_x86(ptr)){
            ft_memcpy(&fat, ptr, sizeof(fat));
            ft_swap_fat_arch(&fat);
            print_fat_arch(fat);
            printf("______________");
            ptr += sizeof(fat);
        }
        else if (is_x86_64(ptr)){
            ft_memcpy(&fat64, ptr, sizeof(fat64));
            printf("x86_64!\n");
            ft_swap_fat_arch_64(&fat64);
            print_fat_arch64(fat64);
            ptr += sizeof(fat64);
            printf("______________");
        } else {
            printf("Weird architecture\n");
            return (-1);
        }
    }
    return(0);
    ft_puts("END_______________________");
    return(0);
}
