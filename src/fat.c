#include "../include/nm.h"

static void ft_swap_fat_header(struct fat_header *fat)
{
    fat->magic = ft_swapint(fat->magic);
    fat->nfat_arch = ft_swapint(fat->nfat_arch);
}

// static void ft_swap_fat_arch(struct fat_arch *arch)
// {
//     arch->cputype = ft_swapint(arch->cputype);
//     arch->cpusubtype = ft_swapint(arch->cpusubtype);
//     arch->offset = ft_swapint(arch->offset);
//     arch->size = ft_swapint(arch->size);
//     arch->align = ft_swapint(arch->align);
// }

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
    printf("arch.cpusubtype : %u\n", ft_swapint(arch.cpusubtype));
    printf("arch.offset : %u\n", arch.offset);
    printf("arch.size : %u\n", arch.size);
    printf("arch.align : %u\n", arch.align);
}
static void print_fat_arch64(struct fat_arch_64 arch)
{
    printf("arch.cputype : %u\n", arch.cputype);
    printf("arch.cpusubtype : %u\n", arch.cpusubtype);
    printf("arch.offset : %llu\n", arch.offset);
    printf("arch.size : %llu\n", arch.size);
    printf("arch.align : %u\n", arch.align);
}

int is_x86(const char *ptr)
{
    int i386 = 0x07000001;

    for(int i = 0; i < 4; ++i)
    {
        if (((const char)ptr[i] != *((const char*)&i386 + i)))
            return (0);
    }
    return (1);
}

int is_x86_64(const char *ptr)
{
    int i386 = 0x07000000;

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
    ft_swap_fat_header(&g_mach.fatheader);
    printf("nfat_arch %d\n", g_mach.fatheader.nfat_arch);
    ptr += sizeof(struct fat_header);
    for (i = 0; i < g_mach.fatheader.nfat_arch; ++i)
    {
        // for(int i = 0; i < 4; ++i)
        // {
        //     printf("%.2x ", (unsigned int)ptr[i]);
        // }
        printf("i:%i\n", i);
        cpu_type_t cputype = CPU_TYPE_X86;
        printf("%.8x\n", cputype);
        printf("%.8x \n", *(int*)ptr);
        if (is_x86(ptr)){
            printf("x86!!!\n");
            ft_memcpy(&fat, ptr, sizeof(fat));
            // ft_swap_fat_arch(&fat);
            // ft_swap_fat_arch(&fat);
            print_fat_arch(fat);

            swap_fat_arch(&fat, i, LITTLE_ENDIAN);
            print_fat_arch(fat);
            // printf("______________\n");
            ptr += sizeof(fat);
        }
        cputype = CPU_TYPE_X86_64;
        printf("%.8x\n", ft_swapint(cputype));
        printf("%.8x \n", *(int*)ptr);
        if (is_x86_64(ptr)){
            ft_memcpy(&fat64, ptr, sizeof(fat64));
            printf("x86_64!\n");
            ft_swap_fat_arch_64(&fat64);
            print_fat_arch64(fat64);
            ptr += sizeof(fat64);
            printf("______________\n");
        } else {
            printf("Weird\n");
        }
    }
    return(0);
    ft_puts("END_______________________");
    return(0);
}
