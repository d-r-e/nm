#ifndef NM_H
# define NM_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/mman.h>
# include <sys/stat.h>

# include <mach-o/nlist.h>
# include <mach-o/loader.h>
# include <mach-o/fat.h>
# include <mach-o/stab.h>
# include <mach-o/fat.h>
# include <ar.h>

# define BINARY "nm"
# define NO_SUCH_FILE "No such file."
# define PERMISSION_DENIED "Permission denied."
# define STAT_ERROR "Stat read error"
# define FALSE 0
# define TRUE 1

/*
** libft
*/
size_t					ft_strlen(const char *s);
void					ft_putstr(const char *s);
void					ft_puts(const char *s);
void					ft_putnbr(int n);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
void					ft_bzero(void *s, size_t n);
char					*ft_strncpy(char *dest, const char *src, size_t n);
int						ft_memcmp(const void *s1, const void *s2, size_t n);
void					*ft_memcpy(void *str1, const void *str2, size_t n);

/*
** output
*/
int	file_error(const char *file, const char *error);
int	permission_denied(const char *s);
int	strerr(const char *s);

/*
** ft_nm
*/
int                     is_elf(const char *memfile, struct stat *s);
unsigned int            is_mach(const char *memfile, struct stat *s);
int                     analyse_elf(const char *s, const char *path);
struct mach_header_64   get_mach_header64(const char *memfile);
int                     analyse_mach64(void);

/*
** Symbols
*/
int			read_symstr(const char *mem, uint32_t nsyms);
const char	*get_symstr(uint32_t index);
int			read_symtable_64(const char *mem, uint32_t nsyms);

/*
** Segments & sections
*/
int		parse_segment(const char *mem, struct segment_command_64 segment);

typedef struct s_mach {
	int						fd;
	struct mach_header_64	header;
	char					*mem;
	struct stat				s;
	struct symtab_command	symtab;
}	t_mach;

t_mach	g_mach;

#endif
