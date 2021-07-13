#ifndef NM_H
# define NM_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <errno.h>
# include <sys/mman.h>
# define NO_SUCH_FILE "No such file"
# define PERMISSION_DENIED "Permission denied."
# define FALSE 0
# define TRUE 1
/*
** libft
*/
size_t	ft_strlen(const char *s);
void	ft_putstr(const char *s);
void	ft_puts(const char *s);
void	ft_putnbr(int n);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	ft_bzero(void *s, size_t n);

/*
** output
*/
int	no_such_file(const char *s);
int permission_denied(const char *s);
int strerr(const char *s);

/*
** ft_nm
*/
int		is_elf(const char *memfile, struct stat *s);
int		analyse(const char *s);
#endif