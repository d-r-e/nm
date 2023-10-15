#include "libft.h"

int ft_getopt(int argc, char *const argv[], const char *optstring, int *optind)
{
    static int optpos = 1;

    if (*optind >= argc || argv[*optind][0] != '-' || argv[*optind][1] == '\0')
        return (-1);
    if (argv[*optind][optpos] == '\0')
    {
        (*optind)++;
        optpos = 1;
        return (ft_getopt(argc, argv, optstring, optind));
    }
    if (ft_strchr(optstring, argv[*optind][optpos]) == NULL)
    {
        ft_putstr_fd("Illegal option -- ", 2);
        ft_putchar_fd(argv[*optind][optpos], 2);
        ft_putchar_fd('\n', 2);
        return ('?');
    }
    return (argv[*optind][optpos++]);
}
