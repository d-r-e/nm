#ifndef NM_H
# define NM_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <elf.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <libft.h>
# include <sys/types.h>
# include <errno.h>
# include <stdbool.h>
# define PROGRAM_NAME "nm"

# define FLAG_A 0x1
# define FLAG_R 0x2
# define FLAG_P 0x4
# define FLAG_U 0x8

typedef struct s_symbol
{
    Elf64_Sym *sym;
    char *name;
    char type;
    char *value;
    char *shndx;
    struct s_symbol *next;
} t_symbol;

#endif