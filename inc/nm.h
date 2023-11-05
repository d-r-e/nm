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
# define FLAG_G 0x10

typedef struct s_symbol
{
    Elf64_Sym *sym;
    Elf32_Sym *sym32;
    char *name;
    char type;
    char *value;
    char *shndx;
    struct s_symbol *next;
} t_symbol;

char _get_symbol_char(Elf64_Sym sym, Elf64_Shdr* shdr, size_t);
char _get_symbol_char32(Elf32_Sym sym, Elf32_Shdr* shdr, size_t);
void print_Elf64_Shdr(Elf64_Shdr* shdr);
void print_type_bind_shn(Elf64_Shdr* shdr,
						 unsigned char type,
						 unsigned char bind);
t_symbol* _sort64(t_symbol* symbols, int flags);
t_symbol* _sort32(t_symbol* symbols, int flags);
#endif